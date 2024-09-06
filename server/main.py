import os, sys

import corewrap


def main(dll_path: str) -> int:
    corewrap.load_core(dll_path)
    corewrap.create_engine()
    corewrap.load_project("assets/project.yml", os.getcwd() + "/")
    asset = corewrap.get_packed_data_from_index(40)  # test_model2.yml
    print(len(asset), flush=True)
    print(asset, flush=True)

    return 0


if __name__ == "__main__":
    exit(main(sys.argv[1]))
