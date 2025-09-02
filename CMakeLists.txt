cmake_minimum_required(VERSION 3.5.0...3.31.2)
find_package(Git)
execute_process(
    COMMAND
        ${GIT_EXECUTABLE} describe --tags --dirty --abbrev=0 --match "v*.*.*"
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_DESCRIBE_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    COMMAND_ERROR_IS_FATAL ANY
)

string(REGEX REPLACE "^v" "" PROJECT_VERSION ${GIT_DESCRIBE_VERSION})
string(REGEX REPLACE "-dirty" ".1" PROJECT_VERSION ${PROJECT_VERSION})
project(TrueWorldCompletion VERSION ${PROJECT_VERSION} LANGUAGES C CXX)

find_package(rcgg-nexus-api)
find_package(imgui)
find_package(nlohmann_json)
find_package(magic_enum)
find_package(enumflags)

include(${CMAKE_CURRENT_SOURCE_DIR}/src-re/GW2RE.cmake)
file(GLOB_RECURSE SRCs ${CMAKE_CURRENT_SOURCE_DIR}/src/*.[hc]pp)
add_library(${PROJECT_NAME}
    SHARED
        ${SRCs}
        ${RE_SRCs}
)

target_link_libraries(${PROJECT_NAME}
    rcgg-nexus-api::rcgg-nexus-api
    imgui::imgui
    nlohmann_json::nlohmann_json
    magic_enum::magic_enum
    EnumFlags::EnumFlags
    ${RE_LIBs}
)
string(REGEX REPLACE "[^A-Z]" "" PROJECT_ACRONYM "${PROJECT_NAME}")
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        ADDON_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        ADDON_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        ADDON_VERSION_PATCH=${PROJECT_VERSION_PATCH}
        ADDON_VERSION_REVISION=$<IF:$<BOOL:${PROJECT_VERSION_TWEAK}>,${PROJECT_VERSION_TWEAK},0>
        ADDON_NAME="${PROJECT_NAME}"
        ADDON_ACRONYM="${PROJECT_ACRONYM}"
)
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${RE_INCLUDE_DIRS}
)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "True_World_Completion")
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "."
)
