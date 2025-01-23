set_project("mystl")
set_version("0.1.0")

set_toolchains("clang")
add_rules("mode.debug", "mode.release")
set_languages("c++20")

target("mystl")
    set_kind("static")
    add_files("modules/**.cppm", {public = true})

for _, file in ipairs(os.files("tests/test_*.cpp")) do
    local name = path.basename(file)
    target(name)
        set_kind("binary")
        add_deps("mystl")
        set_default(false)
        add_files(file)
        add_tests("default")
end
