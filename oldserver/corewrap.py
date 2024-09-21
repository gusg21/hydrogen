import ctypes
import _ctypes

core: ctypes.CDLL


class PackedAsset:
    def __init__(self, packed_asset_pointer: ctypes.c_void_p, asset_index: int):
        print("Created PackedAsset with data @ {}".format(packed_asset_pointer))
        self.packed_asset_pointer = packed_asset_pointer
        self.asset_index = asset_index

    def __del__(self):
        print(f"Deleting packed asset @ {self.packed_asset_pointer}", flush=True)
        core.delete_packed_asset(self.packed_asset_pointer)

    def __iter__(self):
        return (self[i] for i in range(len(self)))

    def __getitem__(self, item: int) -> ctypes.c_byte | None:
        if item < len(self):
            return core.get_data_in_packed_asset(self.packed_asset_pointer, ctypes.c_uint32(item))
        else:
            return None

    def __len__(self) -> int:
        return core.get_length_of_packed_asset(self.asset_index)

    def to_bytearray(self) -> bytearray:
        print("Converting PackedAsset @ {} to bytearray...".format(self.packed_asset_pointer))
        length = len(self)
        print("Byte length: {}".format(length))
        data = bytearray(self)
        print("Done conversion")
        return data

    def __repr__(self):
        return '<PackedAsset @ {}>'.format(self.packed_asset_pointer)


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
    core.get_data_in_packed_asset.restype = ctypes.c_ubyte
    core.get_data_in_packed_asset.argtypes = [ctypes.c_void_p, ctypes.c_uint32]
    core.get_length_of_packed_asset.restype = ctypes.c_size_t
    core.get_length_of_packed_asset.argtypes = [ctypes.c_uint32]


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
    print("Getting packed asset for index {}".format(asset_index))
    return PackedAsset(core.get_packed_asset_from_index(asset_index), asset_index)
