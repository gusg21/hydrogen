import ctypes
import _ctypes

core: ctypes.CDLL
c_str = ctypes.POINTER(ctypes.c_char)


class PackedAsset:
    def __init__(self, packed_asset_pointer: ctypes.c_void_p):
        print(f"Got packed asset @ {packed_asset_pointer}", flush=True)
        self.packed_asset_pointer = packed_asset_pointer

    def __del__(self):
        print(f"Deleting packed asset @ {self.packed_asset_pointer}", flush=True)
        core.delete_packed_asset(self.packed_asset_pointer)

    def __getitem__(self, item: int):
        return core.get_data_in_packed_asset(self.packed_asset_pointer, ctypes.c_uint32(item))

    def __len__(self):
        return core.get_length_of_packed_asset(self.packed_asset_pointer)

    def to_bytes(self) -> bytes:
        data = bytes()
        for i in range(len(self)):
            data += self[i][0:1]
        return data

    def __repr__(self):
        return '[{}]'.format(', '.join("0x{0:x}".format(self[i][0]) for i in range(len(self))))


def load_core(dll_path: str) -> None:
    global core

    if not dll_path:
        raise TypeError("Invalid DLL path")

    handle = _ctypes.LoadLibrary(dll_path)
    core = ctypes.CDLL(dll_path, handle=handle) if handle != 0 else handle

    core.create_engine.restype = None
    core.create_engine.argtypes = []
    core.load_project.restype = None
    core.load_project.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
    core.get_loaded_asset_count.restype = ctypes.c_uint32
    core.get_loaded_asset_count.argtypes = []
    core.get_max_asset_count.restype = ctypes.c_uint32
    core.get_max_asset_count.argtypes = []
    core.is_packed_asset_index_valid.restype = ctypes.c_bool
    core.is_packed_asset_index_valid.argtypes = [ctypes.c_uint32]
    core.get_packed_asset_from_index.restype = ctypes.c_void_p
    core.get_packed_asset_from_index.argtypes = [ctypes.c_uint32]
    core.delete_packed_asset.restype = None
    core.delete_packed_asset.argtypes = [ctypes.c_void_p]
    core.get_data_in_packed_asset.restype = ctypes.c_char
    core.get_data_in_packed_asset.argtypes = [ctypes.c_void_p]
    core.get_length_of_packed_asset.restype = ctypes.c_size_t
    core.get_length_of_packed_asset.argtypes = [ctypes.c_void_p]


def create_engine() -> None:
    core.create_engine()


def load_project(project_file: str, assets_base_path: str) -> None:
    core.load_project(ctypes.c_char_p(project_file.encode("utf-8")), ctypes.c_char_p(assets_base_path.encode("utf-8")))


def get_loaded_asset_count() -> int:
    return core.get_loaded_asset_count()


def get_max_asset_count() -> int:
    return core.get_max_asset_count()


def is_packed_asset_index_valid(asset_index: int) -> bool:
    return core.is_packed_asset_index_valid(asset_index)


def get_packed_asset_from_index(asset_index: int) -> PackedAsset:
    return PackedAsset(core.get_packed_asset_from_index(asset_index))
