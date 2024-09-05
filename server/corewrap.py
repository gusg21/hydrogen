import ctypes

core = None
c_str = ctypes.POINTER(ctypes.c_char)

def load_core(dll_path: str) -> ctypes.CDLL:
    global core
    
    core = ctypes.CDLL(dll_path)

def create_engine() -> None:
    core.create_engine()    

def load_project(project_file: str) -> None:
    core.load_project(ctypes.c_char_p(project_file.encode()))