import os, sys

import corewrap

def main(build_folder: str = "cmake-build-debug-visual-studio") -> int:
    if sys.platform.startswith("win32"):
        corewrap.load_core(f"../{build_folder}/server/HydrogenServer_Wrapper.dll")
    elif sys.platform.startswith("darwin"):
        corewrap.load_core(f"../{build_folder}/server/libHydrogenServer_Wrapper.dylib")
    elif sys.platform.startswith("linux"):
        corewrap.load_core(f"../{build_folder}/server/libHydrogenServer_Wrapper.so")
    else:
        print("Unable to determine OS/library file extension")
        return 1

    corewrap.create_engine()
    corewrap.load_project("assets/project.yml")

if __name__ == "__main__":
    exit(main())