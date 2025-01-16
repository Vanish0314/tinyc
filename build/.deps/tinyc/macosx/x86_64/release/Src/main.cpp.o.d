{
    values = {
        "/usr/local/Cellar/llvm/19.1.6/bin/clang",
        {
            "-Qunused-arguments",
            "-m64",
            "-isysroot",
            "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk",
            "-fvisibility=hidden",
            "-fvisibility-inlines-hidden",
            "-O3",
            "-std=c++17",
            "-ISrc",
            "-isystem",
            "/Users/vanish/.xmake/packages/t/tinygltf/v2.9.3/c0a5094c611b4ceab086c57a7a5d6fba/include",
            "-isystem",
            "/Users/vanish/.xmake/packages/s/stb/2024.06.01/c5dad42a0d624f5a9cc4dce48178a35e/include",
            "-isystem",
            "/Users/vanish/.xmake/packages/s/stb/2024.06.01/c5dad42a0d624f5a9cc4dce48178a35e/include/stb",
            "-isystem",
            "/Users/vanish/.xmake/packages/n/nlohmann_json/v3.11.3/07dbd8c7f12347f583937915168be7dc/include",
            "-DNDEBUG"
        }
    },
    files = {
        "Src/main.cpp"
    },
    depfiles_gcc = "build/.objs/tinyc/macosx/x86_64/release/Src/__cpp_main.cpp.cpp:   Src/main.cpp Src/cgltf.h\
"
}