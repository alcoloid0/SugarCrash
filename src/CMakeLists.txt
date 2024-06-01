file(GLOB C_SOURCES *.c)

add_library(SugarCrashLibrary STATIC)

target_sources(SugarCrashLibrary PRIVATE ${C_SOURCES})

target_link_libraries(SugarCrashLibrary PRIVATE
    winmm psapi
)

target_compile_definitions(SugarCrashLibrary PRIVATE
    _UNICODE UNICODE $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(SugarCrashLibrary PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SugarCrashLibrary PUBLIC ntdll)

if(MINGW)
    target_link_libraries(SugarCrashLibrary PRIVATE "-static")
endif()

add_executable(SugarCrash WIN32
    executable/main.c
    ${PROJECT_SOURCE_DIR}/resources/resources.rc
)

target_link_libraries(SugarCrash PRIVATE SugarCrashLibrary)

set_target_properties(SugarCrash PROPERTIES
    WIN32_EXECUTABLE TRUE
)