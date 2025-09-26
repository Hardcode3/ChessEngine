
# constexpr usage with fmt requires the use of /utf-8 compile option on MSVC
if (MSVC)
    add_compile_options(/utf-8)
endif()
