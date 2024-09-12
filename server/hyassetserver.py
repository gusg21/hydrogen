import os
import time

import flask

import corewrap


class HyAssetServer():
    app = flask.Flask("server")
    load_simulation_time = 0

    def __init__(self, dll_path: str, project_path: str = "assets/project.yml", load_simulation_time: float = 0):
        HyAssetServer.load_simulation_time = load_simulation_time

        corewrap.load_core(dll_path)
        corewrap.create_engine()
        corewrap.load_project(project_path, os.getcwd() + "/")

    def run(self, host, port):
        HyAssetServer.app.run(host, port)


@HyAssetServer.app.route("/")
def status_page():
    return flask.render_template("status.html", loaded_asset_count=corewrap.get_loaded_asset_count(),
                                 max_asset_count=corewrap.get_max_asset_count())


@HyAssetServer.app.route("/asset/", defaults={"asset_id": -1})
@HyAssetServer.app.route("/asset/<int:asset_id>")
def get_asset(asset_id: int):
    if asset_id < 0:
        return flask.render_template("bad_asset_get.html", asset_id=asset_id), 404

    if corewrap.is_packed_asset_index_valid(asset_id):
        print("Serving asset {}...".format(asset_id))
        if HyAssetServer.load_simulation_time > 0:
            print("Simulating heavy load...")
            time.sleep(HyAssetServer.load_simulation_time)
        asset_bytes = corewrap.get_packed_asset_from_index(asset_id).to_bytes()
        print(len(asset_bytes))
        return flask.Response(
            asset_bytes, mimetype="bin/hya"
        ), 200
    else:
        return flask.render_template("bad_asset_get.html", asset_id=asset_id), 404


@HyAssetServer.app.errorhandler(404)
def not_found(e):
    return flask.render_template("404.html"), 404
