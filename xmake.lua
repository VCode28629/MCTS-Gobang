set_languages("c++17")
set_optimize("fastest")

target("main")
    set_kind("binary")
    add_files("*.cpp")
