#!/usr/bin/env python3
import os
import platform
import shutil
import struct
import subprocess
import sys
from pathlib import Path

JADIDI_HOME = Path.home() / ".jadidi"
SOL2_REPO = "https://github.com/ThePhD/sol2.git"
SOL2_TAG = "v3.5.0"

DEPENDENCIES = {
    "debian": [
        "build-essential",
        "cmake",
        "git",
        "pkg-config",
        "libsdl2-dev",
        "libsdl2-image-dev",
        "libsdl2-ttf-dev",
        "libsdl2-mixer-dev",
        "liblua5.4-dev",
        "libbox2d-dev",
    ],
    "fedora": [
        "gcc-c++",
        "cmake",
        "git",
        "pkgconf-pkg-config",
        "SDL2-devel",
        "SDL2_image-devel",
        "SDL2_ttf-devel",
        "SDL2_mixer-devel",
        "lua-devel",
        "box2d-devel",
    ],
    "arch": [
        "base-devel",
        "cmake",
        "git",
        "pkgconf",
        "sdl2",
        "sdl2_image",
        "sdl2_ttf",
        "sdl2_mixer",
        "lua",
        "box2d",
    ],
    "void": [
        "base-devel",
        "cmake",
        "git",
        "pkg-config",
        "SDL2-devel",
        "SDL2_image-devel",
        "SDL2_ttf-devel",
        "SDL2_mixer-devel",
        "lua54-devel",
        "box2d-devel",
    ],
}

FAMILY_MAP = {
    "debian": "debian",
    "ubuntu": "debian",
    "linuxmint": "debian",
    "pop": "debian",
    "fedora": "fedora",
    "nobara": "fedora",
    "arch": "arch",
    "manjaro": "arch",
    "endeavouros": "arch",
    "cachyos": "arch",
    "void": "void",
}


def die(message):
    print(f"Error: {message}", file=sys.stderr)
    sys.exit(1)


def run(cmd, **kwargs):
    print("$", " ".join(map(str, cmd)))
    subprocess.run(cmd, check=True, **kwargs)


def with_sudo(cmd):
    if os.geteuid() == 0:
        return cmd

    if shutil.which("sudo"):
        return ["sudo", *cmd]

    return cmd


def detect_distro():
    if platform.system() != "Linux":
        die("This tool only supports Linux.")

    os_release = Path("/etc/os-release")

    if not os_release.exists():
        die("Cannot detect Linux distribution.")

    values = {}

    for line in os_release.read_text().splitlines():
        line = line.strip()

        if not line or "=" not in line:
            continue

        key, value = line.split("=", 1)
        values[key] = value.strip().strip('"')

    candidates = []

    if values.get("ID"):
        candidates.append(values["ID"])

    if values.get("ID_LIKE"):
        candidates.extend(values["ID_LIKE"].lower().split())

    for candidate in candidates:
        family = FAMILY_MAP.get(candidate.lower())

        if family:
            return family

    die(f"Unsupported distribution: {values.get('ID', 'unknown')}")


def install_deps(distro):
    packages = DEPENDENCIES[distro]

    print(f"Installing dependencies for {distro}...")

    if distro == "debian":
        run(with_sudo(["apt", "update"]))
        run(with_sudo(["apt", "install", "-y", *packages]))

    elif distro == "fedora":
        run(with_sudo(["dnf", "install", "-y", *packages]))

    elif distro == "arch":
        run(with_sudo([
            "pacman",
            "-S",
            "--needed",
            "--noconfirm",
            *packages,
        ]))

    elif distro == "void":
        run(with_sudo(["xbps-install", "-Su"]))
        run(with_sudo(["xbps-install", "-y", *packages]))

    print("Dependencies installed.")


def install_sol2():
    include_dir = JADIDI_HOME / "include"
    sol2_header = include_dir / "sol" / "sol.hpp"

    if sol2_header.exists():
        print("sol2 already installed.")
        return

    print("Installing sol2...")

    sol2_source = JADIDI_HOME / "sources" / "sol2"
    sol2_source.parent.mkdir(parents=True, exist_ok=True)

    if sol2_source.exists():
        shutil.rmtree(sol2_source)

    run([
        "git",
        "clone",
        "--depth", "1",
        "--branch", SOL2_TAG,
        SOL2_REPO,
        str(sol2_source),
    ])

    include_dir.mkdir(parents=True, exist_ok=True)
    target = include_dir / "sol"
    if target.exists():
        shutil.rmtree(target)
    shutil.copytree(sol2_source / "include" / "sol", target)

    cmake_dir = JADIDI_HOME / "lib" / "cmake" / "sol2"
    cmake_dir.mkdir(parents=True, exist_ok=True)

    config_content = (
        'set(SOL2_VERSION "' + SOL2_TAG + '")\n'
        'set(SOL2_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../include")\n'
        '\n'
        'if(NOT TARGET sol2)\n'
        '    add_library(sol2 INTERFACE IMPORTED)\n'
        '    set_target_properties(sol2 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${SOL2_INCLUDE_DIR}")\n'
        'endif()\n'
        '\n'
        'set(sol2_FOUND TRUE)\n'
    )

    (cmake_dir / "sol2Config.cmake").write_text(config_content)

    print(f"sol2 installed: {target}")


def make_bmp():
    header = struct.pack(
        "<2sIHHI",
        b"BM",
        58,
        0,
        0,
        54,
    )

    info = struct.pack(
        "<IiiHHIIiiII",
        40,
        1,
        1,
        1,
        24,
        0,
        4,
        2835,
        2835,
        0,
        0,
    )

    pixel = b"\x00\x00\xff\x00"

    return header + info + pixel


def create_project_structure(build_dir, repo_root):
    print("Creating project structure...")

    if build_dir.exists():
        shutil.rmtree(build_dir)

    build_dir.mkdir(parents=True)

    for directory in ("Fonts", "Scenes", "Scripts", "Shaders"):
        (build_dir / directory).mkdir()

    config = {
        "window": {
            "fullscreen": False,
            "height": 720,
            "icon": "icon.bmp",
            "renderer": "opengl",
            "title": "jadidi",
            "width": 1280,
        }
    }

    import json

    (build_dir / "config.json").write_text(
        json.dumps(config, indent=4) + "\n"
    )

    (build_dir / "icon.bmp").write_bytes(make_bmp())
    (build_dir / "Fonts" / "font.ttf").write_bytes(b"")
    (build_dir / "Scenes" / "home.json").write_text(
        '{ "objects": [] }\n'
    )

    vert = """#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 projection;
uniform vec2 uvOffset;
uniform vec2 uvScale;

void main() {
    gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord * uvScale + uvOffset;
}
"""

    frag = """#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform bool useTexture;

void main() {
    if (useTexture) {
        FragColor = spriteColor * texture(image, TexCoord);
    } else {
        FragColor = spriteColor;
    }
}
"""

    (build_dir / "Shaders" / "sprite.vert").write_text(vert)
    (build_dir / "Shaders" / "sprite.frag").write_text(frag)

    ide_src = repo_root / "ide autocompletion"

    if ide_src.exists():
        shutil.copytree(
            ide_src,
            build_dir / "ide autocompletion",
        )
    else:
        print(
            "Warning: 'ide autocompletion' "
            "not found in repository root."
        )

    print(f"Project structure created: {build_dir}")


def build_engine(repo_root, build_dir):
    print("Building engine...")

    cmake_build_dir = build_dir / ".cmake-build"

    run([
        "cmake",
        "-S",
        str(repo_root),
        "-B",
        str(cmake_build_dir),
        "-DCMAKE_BUILD_TYPE=Release",
        "-DJADIDI_BUILD_PROFILE=generic",
    ])

    run([
        "cmake",
        "--build",
        str(cmake_build_dir),
        "--parallel",
    ])

    binary = cmake_build_dir / "jadidi"

    if not binary.exists():
        die("Build succeeded but binary not found.")

    shutil.copy2(binary, build_dir / "jadidi")
    (build_dir / "jadidi").chmod(0o755)

    print(f"Binary: {build_dir / 'jadidi'}")


def main():
    if platform.system() != "Linux":
        die("This tool only supports Linux.")

    repo_root = Path(__file__).resolve().parent

    if not (repo_root / "CMakeLists.txt").exists():
        die(
            "CMakeLists.txt not found. "
            "Run this script from the repository root."
        )

    distro = detect_distro()

    install_deps(distro)
    install_sol2()

    build_dir = repo_root / "build"

    create_project_structure(build_dir, repo_root)
    build_engine(repo_root, build_dir)

    print("")
    print("Done! Run your project:")
    print(f"  cd {build_dir}")
    print("  ./jadidi")


if __name__ == "__main__":
    main()
