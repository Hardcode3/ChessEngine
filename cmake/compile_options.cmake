
if (MSVC)
    # constexpr usage with fmt requires the use of /utf-8 compile option on MSVC
    # [build] vcpkg_installed\x64-windows\include\fmt\base.h(458,28):
    #   error C2338: static_assert failed:
    #       'Unicode support requires compiling with /utf-8'
    add_compile_options(/utf-8)
endif()
