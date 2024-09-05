import ctypes

core: ctypes.CDLL
c_str = ctypes.POINTER(ctypes.c_char)

def load_core(dll_path: str) -> None:
    global core
    
    core = ctypes.cdll.LoadLibrary(dll_path)

    core.load_project.argtypes = [ctypes.c_char_p]


def create_engine() -> None:
    core.create_engine()    

def load_project(project_file: str) -> None:
    core.load_project(ctypes.c_char_p(project_file.encode("utf-8")))