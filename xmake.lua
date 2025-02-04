set_languages("c++17")
add_cflags("-O3")

target("main")
    set_kind("binary")
    add_files("*.cpp")
