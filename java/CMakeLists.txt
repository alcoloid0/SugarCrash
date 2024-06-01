set(CMAKE_JAVA_COMPILE_FLAGS "-source" "1.8" "-target" "1.8")
set(PREPARE_JAR_DIR ${CMAKE_BINARY_DIR}/prepare_jar)
set(JAR_NAME ${PROJECT_NAME}Bukkit)
set(DLL_NAME ${PROJECT_NAME}JNI)

find_package(Java REQUIRED)
find_package(JNI REQUIRED)

include(UseJava)

file(GLOB_RECURSE JAVA_SOURCES *.java)

add_jar(SugarCrashBukkit
    SOURCES ${JAVA_SOURCES}
    GENERATE_NATIVE_HEADERS SugarCrashJavaNative
    OUTPUT_NAME ${JAR_NAME}
)

add_custom_command(TARGET SugarCrashBukkit POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E tar xzf
        ${CMAKE_CURRENT_BINARY_DIR}/${JAR_NAME}.jar
    COMMAND ${CMAKE_COMMAND} -E remove_directory org/
    WORKING_DIRECTORY ${PREPARE_JAR_DIR}
    COMMENT "Unpack and prepare jar"
)

add_library(SugarCrashLibraryJavaNative SHARED
    jni/native.c
    ${PROJECT_SOURCE_DIR}/resources/resources.rc
)

target_link_libraries(SugarCrashLibraryJavaNative PRIVATE
    JNI::JNI
    SugarCrashJavaNative
    SugarCrashLibrary
)

set_target_properties(SugarCrashLibraryJavaNative PROPERTIES
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PREFIX ""
    RUNTIME_OUTPUT_DIRECTORY ${PREPARE_JAR_DIR}
    OUTPUT_NAME ${DLL_NAME}
)

configure_file(
    ${PROJECT_SOURCE_DIR}/resources/plugin.yml.in
    ${PREPARE_JAR_DIR}/plugin.yml
)

if(MSVC)
    add_custom_command(TARGET SugarCrashLibraryJavaNative POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<CONFIG>/${DLL_NAME}.dll ${DLL_NAME}.dll
        COMMAND ${CMAKE_COMMAND} -E tar cfv
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/${JAR_NAME}.jar --format=zip
            "META-INF/" "omg/" "cz/" "plugin.yml" "${DLL_NAME}.dll"
        WORKING_DIRECTORY ${PREPARE_JAR_DIR}
        COMMENT "Repack jar (MSVC)"
    )
else()
    add_custom_command(TARGET SugarCrashLibraryJavaNative POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E tar cfv
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${JAR_NAME}.jar --format=zip
            "META-INF/" "omg/" "cz/" "plugin.yml" "${DLL_NAME}.dll"
        WORKING_DIRECTORY ${PREPARE_JAR_DIR}
        COMMENT "Repack jar"
    )
endif()