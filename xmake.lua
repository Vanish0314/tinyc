-- cpp standard
set_languages("c++17")

-- Build Mode
add_rules("mode.release","mode.debug")

-- toolchains requirements
add_requires("llvm")

-- requirements
add_requires("tinygltf")
add_requires("nlohmann_json")

-- target(.exe)
target("tinyc")
    set_kind("binary")
    set_toolchains("llvm")
    add_includedirs("Src")
    add_files("Src/**.cpp")
    add_packages("tinygltf","nlohmann_json")
