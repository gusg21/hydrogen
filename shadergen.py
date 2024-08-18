import os, shutil, subprocess

SHADERC = "build/core/external/bgfx.cmake/cmake/bgfx/Debug/shaderc.exe"


def shaderc(platform, profile, shader_name):
    os.mkdir("core/generated/{}".format(platform))
    for shader_type_info in [("fs", "fragment"), ("vs", "vertex")]:
        print("> shader {} type {} for platform {}".format(shader_name, shader_type_info[1], platform))
        cmd = '"{}" --platform {} -p {} --verbose --depends -O 3 --disasm -f core/shaders/{}_{}.sc -o core/generated/{}/{}_{}.bin -i core/external/bgfx.cmake/bgfx/src/ --type {} --varyingdef core/shaders/varying.def.sc'.format(
            SHADERC,
            platform,
            profile,
            shader_type_info[0],
            shader_name,
            platform,
            shader_type_info[0],
            shader_name,
            shader_type_info[1],
        )

        print(cmd)
        subprocess.call(cmd)


def generate_per_platform_shaders(shader_name):
    print('Generating platform shader code for shader "{}"...'.format(shader_name))
    print()

    platforms = [("windows", "s_5_0"), ("osx", "metal"), ("linux", "spirv")]

    for platform_pair in platforms:
        shaderc(platform_pair[0], platform_pair[1], shader_name)


def clean():
    shutil.rmtree("core/generated")
    os.mkdir("core/generated")


def main():
    clean()
    generate_per_platform_shaders("default")


if __name__ == "__main__":
    main()
