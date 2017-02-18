
get_cmake_property(is_set_up _CMAKETOOLS_SET_UP)
if(NOT is_set_up)
    set_property(GLOBAL PROPERTY _CMAKETOOLS_SET_UP TRUE)
    macro(_cmt_invoke fn)
        file(WRITE "${CMAKE_BINARY_DIR}/_cmt_tmp.cmake" "
            set(_args \"${ARGN}\")
            ${fn}(\${_args})
        ")
        include("${CMAKE_BINARY_DIR}/_cmt_tmp.cmake" NO_POLICY_SCOPE)
    endmacro()

    set(_cmt_add_executable add_executable)
    set(_previous_cmt_add_executable _add_executable)
    while(COMMAND "${_previous_cmt_add_executable}")
        set(_cmt_add_executable "_${_cmt_add_executable}")
        set(_previous_cmt_add_executable _${_previous_cmt_add_executable})
    endwhile()
    macro(${_cmt_add_executable} target)
        _cmt_invoke(${_previous_cmt_add_executable} ${ARGV})
        get_target_property(is_imported ${target} IMPORTED)
        if(NOT is_imported)
            file(APPEND
                "${CMAKE_BINARY_DIR}/CMakeToolsMeta.in.txt"
                "executable;${target};$<TARGET_FILE:${target}>
"
                )
            _cmt_generate_system_info()
        endif()
    endmacro()

    set(_cmt_add_library add_library)
    set(_previous_cmt_add_library _add_library)
    while(COMMAND "${_previous_cmt_add_library}")
        set(_cmt_add_library "_${_cmt_add_library}")
        set(_previous_cmt_add_library "_${_previous_cmt_add_library}")
    endwhile()
    macro(${_cmt_add_library} target)
        _cmt_invoke(${_previous_cmt_add_library} ${ARGV})
        get_target_property(type ${target} TYPE)
        if(NOT type MATCHES "^(INTERFACE_LIBRARY|OBJECT_LIBRARY)$")
            get_target_property(imported ${target} IMPORTED)
            get_target_property(alias ${target} ALIAS)
            if(NOT imported AND NOT alias)
                file(APPEND
                    "${CMAKE_BINARY_DIR}/CMakeToolsMeta.in.txt"
                    "library;${target};$<TARGET_FILE:${target}>
"
                    )
            endif()
        else()
            file(APPEND
                "${CMAKE_BINARY_DIR}/CMakeToolsMeta.in.txt"
                "interface-library;${target}
"
                )
        endif()
        _cmt_generate_system_info()
    endmacro()

    if(0)
        set(condition CONDITION "$<CONFIG:Debug>")
    endif()

    file(WRITE "${CMAKE_BINARY_DIR}/CMakeToolsMeta.in.txt" "")
    file(GENERATE
        OUTPUT "${CMAKE_BINARY_DIR}/CMakeToolsMeta-$<CONFIG>.txt"
        INPUT "${CMAKE_BINARY_DIR}/CMakeToolsMeta.in.txt"
        ${condition}
        )

    function(_cmt_generate_system_info)
        get_property(done GLOBAL PROPERTY CMT_GENERATED_SYSTEM_INFO)
        if(NOT done)
            set(_compiler_id "${CMAKE_CXX_COMPILER_ID}")
            if(MSVC AND CMAKE_CXX_COMPILER MATCHES ".*clang-cl.*")
                set(_compiler_id "MSVC")
            endif()
            file(APPEND "${CMAKE_BINARY_DIR}/CMakeToolsMeta.in.txt"
    "system;${CMAKE_HOST_SYSTEM_NAME};${CMAKE_SYSTEM_PROCESSOR};${_compiler_id}
")
        endif()
        set_property(GLOBAL PROPERTY CMT_GENERATED_SYSTEM_INFO TRUE)
    endfunction()
endif()
