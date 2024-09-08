import os
import time

import flask

import corewrap


class HyAssetServer():
    app = flask.Flask("server")

    def __init__(self, dll_path: str, project_path: str = "assets/project.yml"):
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
        print("Simulating heavy load...")
        time.sleep(7)
        return flask.Response(
            corewrap.get_packed_asset_from_index(asset_id).to_bytes(), mimetype="bin/hya"
        ), 200
    else:
        return flask.render_template("bad_asset_get.html", asset_id=asset_id), 404


@HyAssetServer.app.errorhandler(404)
def not_found(e):
    return flask.render_template("404.html"), 404