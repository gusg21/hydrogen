import os

import flask

import corewrap


# corewrap.load_core(dll_path)
# corewrap.create_engine()
# corewrap.load_project("assets/project.yml", os.getcwd() + "/")
# asset = corewrap.get_packed_data_from_index(40)  # test_model2.yml
# print(len(asset), flush=True)
# print(asset, flush=True)

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


@HyAssetServer.app.route("/asset/<int:asset_id>")
def get_asset(asset_id: int):
    return flask.Response(
        corewrap.get_packed_data_from_index(asset_id).to_bytes(), mimetype="bin/hya"
    )
