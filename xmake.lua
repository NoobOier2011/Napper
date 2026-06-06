add_rules("mode.debug", "mode.release")
set_languages("cxx17")

target("Napper")
    set_kind("binary")
    add_files("src/*.cpp")

    if is_plat("windows") then
        -- sdk & lib
        add_rules("win.sdk.application")
        set_policy("build.win.win32_subsystem", "windows")
        
        local arch_dir = is_arch("x64") and "x64" or "x86"
        add_linkdirs(path.join("third_party/lib", arch_dir))
        add_links("WebView2Loader.dll")

        -- third party
        add_includedirs("third_party/include")

        after_build(function(target)
            local output_dir = target:targetdir()
            local loader_dll = path.join("third_party/lib", arch_dir, "WebView2Loader.dll")
            if os.isfile(loader_dll) then
                os.cp(loader_dll, path.join(output_dir, "WebView2Loader.dll"))
            end

            -- Copy web view
            if os.isdir("web") then
                os.cp("web", path.join(output_dir, "web"))
            end
        end)
    end