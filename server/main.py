import corewrap

def main():
    corewrap.load_core("../build/server/libHydrogenCore_Wrapper.dylib")
    corewrap.create_engine()
    corewrap.load_project("assets/project.yml")

if __name__ == "__main__":
    main()