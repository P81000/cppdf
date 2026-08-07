# cmake/Profiles.cmake

set(_CPPDF_WARN
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
    -Wsign-conversion
    -Wnull-dereference
    -Wdouble-promotion
    -Wformat=2
    -Wno-unused-parameter
)

set(_CPPDF_FLAGS_DEBUG
    -O0
    -g3
    -fno-omit-frame-pointer
    -fsanitize=address,undefined
    -fno-sanitize-recover=all
)

set(_CPPDF_FLAGS_RELEASE
    -O3
    -DNDEBUG
    -flto
    -fno-omit-frame-pointer
)

set(_CPPDF_FLAGS_PROFILE
    -O2
    -g
    -fno-omit-frame-pointer
)

set(_CPPDF_FLAGS_TSAN
    -O1
    -g
    -fno-omit-frame-pointer
    -fsanitize=thread
)

set(_CPPDF_LINK_DEBUG   -fsanitize=address,undefined)
set(_CPPDF_LINK_RELEASE -flto)
set(_CPPDF_LINK_TSAN    -fsanitize=thread)

function(cppdf_apply_profile target)
    target_compile_options(${target} PRIVATE
        ${_CPPDF_WARN}
        $<$<CONFIG:Debug>:${_CPPDF_FLAGS_DEBUG}>
        $<$<CONFIG:Release>:${_CPPDF_FLAGS_RELEASE}>
        $<$<CONFIG:Profile>:${_CPPDF_FLAGS_PROFILE}>
        $<$<CONFIG:TSan>:${_CPPDF_FLAGS_TSAN}>
    )

    get_target_property(_type ${target} TYPE)
    if(_type STREQUAL "EXECUTABLE")
        target_link_options(${target} PRIVATE
            $<$<CONFIG:Debug>:${_CPPDF_LINK_DEBUG}>
            $<$<CONFIG:Release>:${_CPPDF_LINK_RELEASE}>
            $<$<CONFIG:TSan>:${_CPPDF_LINK_TSAN}>
        )
    endif()
endfunction()
