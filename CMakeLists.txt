#================================
# Project setup.
#================================
cmake_minimum_required(VERSION 3.12)
project(apollo VERSION 1.0.0 LANGUAGES CXX C)

if (APPLE)
    message(ERROR "Apollo does not support Apple platforms.")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Define version variables.
set(APOLLO_VERSION_MAJOR "0")
set(APOLLO_VERSION_MINOR "0")
set(APOLLO_VERSION_PATCH "0")
set(APOLLO_VERSION_EXTRA "")
set(APOLLO_VERSION "${APOLLO_VERSION_MAJOR}.${APOLLO_VERSION_MINOR}")
set(APOLLO_VERSION_FULL
    "${APOLLO_VERSION}.${APOLLO_VERSION_PATCH}${APOLLO_VERSION_EXTRA}")

if (POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

#================================
# Option variables.
#================================
option(APOLLO_BUILD_TESTS "Build Apollo unit tests" ON)
option(APOLLO_BUILD_PARALLEL "Build parallel version with TBB" OFF)
set(APOLLO_REAL_TYPE "float" CACHE STRING "Real type used by Apollo")
set_property(CACHE APOLLO_REAL_TYPE PROPERTY STRINGS "float" "double")

# Validate the option from the user.
get_property(TYPE_STRINGS CACHE APOLLO_REAL_TYPE PROPERTY STRINGS)
if (NOT APOLLO_REAL_TYPE IN_LIST TYPE_STRINGS)
    message(FATAL_ERROR "Invalid real type")
endif()

# Make sure Zeus does not make the clang-format target and disable the
# unit tests for it.
set(ZEUS_BUILD_TESTS OFF CACHE INTERNAL "")

#================================
# Directory variables.
#================================
set(APOLLO_SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(APOLLO_SOURCE_ROOT ${APOLLO_SOURCE_DIR}/src)
set(APOLLO_TEST_ROOT ${APOLLO_SOURCE_DIR}/test)
set(APOLLO_CMAKE_ROOT ${APOLLO_SOURCE_DIR}/cmake)

#================================
# Add subdirectories.
#================================
add_subdirectory(${APOLLO_SOURCE_ROOT})

#================================
# Find Packages.
#================================
include(FetchContent)

# Declare fetch content for all packages.
FetchContent_Declare(
    zeus
    GIT_REPOSITORY https://github.com/marovira/zeus
    GIT_TAG 531afeb2f6c7ac2a7c4057c3d7278868324cc584
    )

FetchContent_Declare(
    lua
    GIT_REPOSITORY https://github.com/marovira/lua
    GIT_TAG cb847f204a1615ee9801874770230290517f760b
    )

find_package(zeus QUIET)

if (NOT zeus_FOUND AND NOT zeus_POPULATED)
    FetchContent_Populate(zeus)
    add_subdirectory(${zeus_SOURCE_DIR} ${zeus_BINARY_DIR})
endif()

if (APOLLO_BUILD_PARALLEL)
    FetchContent_Declare(
        tbb
        GIT_REPOSITORY https://github.com/oneapi-src/oneTBB
        GIT_TAG 2dba2072869a189b9fdab3ffa431d3ea49059a19
        )

    find_package(tbb QUIET)

    if (NOT tbb_FOUND AND NOT tbb_POPULATED)
        FetchContent_Populate(tbb)
        add_subdirectory(${tbb_SOURCE_DIR} ${tbb_BINARY_DIR})
    endif()
endif()

#================================
# Compiler flags.
#================================
if (MSVC)
    set(APOLLO_COMPILER_FLAGS /W4 /WX /MP)
    set(APOLLO_DEBUG_FLAGS "$<$<CONFIG:DEBUG>:/ZI>")
    set(APOLLO_COMPILE_DEFINITIONS -DNOMINMAX)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL
        "GNU")
    set(APOLLO_COMMON_FLAGS -Wall -Wextra -pedantic -Werror)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(APOLLO_COMPILER_FLAGS ${APOLLO_COMMON_FLAGS})
    else()
        set(APOLLO_COMPILER_FLAGS ${APOLLO_COMMON_FLAGS} -stdlib=libstdc++)
    endif()
    set(APOLLO_DEBUG_FLAGS "$<$<CONFIG:DEBUG>:-g>")
endif()

if (APOLLO_BUILD_PARALLEL)
    set(APOLLO_COMPILE_DEFINITIONS ${APOLLO_COMPILE_DEFINITIONS}
        -DAPOLLO_BUILD_PARALLEL)
endif()

if (APOLLO_REAL_TYPE STREQUAL "float")
    set(APOLLO_COMPILE_DEFINITIONS ${APOLLO_COMPILE_DEFINITIONS}
        -DAPOLLO_USE_FLOAT)
elseif(APOLLO_REAL_TYPE STREQUAL "double")
    set(APOLLO_COMPILE_DEFINITIONS ${APOLLO_COMPILE_DEFINITIONS}
        -DAPOLLO_USE_DOUBLE)
endif()

#================================
# Core library.
#================================
source_group("include" FILES ${APOLLO_INCLUDE_CORE_GROUP})
source_group("source" FILES ${APOLLO_SOURCE_CORE_GROUP})

add_library(core ${APOLLO_INCLUDE_CORE_GROUP} ${APOLLO_SOURCE_CORE_GROUP})
target_include_directories(core PUBLIC ${APOLLO_SOURCE_ROOT})
target_link_libraries(core PUBLIC zeus::zeus)
if (NOT MSVC)
    target_link_libraries(core PUBLIC stdc++fs)
endif()
target_compile_features(core PUBLIC cxx_std_17)
target_compile_options(core PUBLIC ${APOLLO_COMPILER_FLAGS})
target_compile_options(core PUBLIC ${APOLLO_DEBUG_FLAGS})
target_compile_definitions(core PUBLIC ${APOLLO_COMPILE_DEFINITIONS})
set_target_properties(core PROPERTIES FOLDER "apollo")

#================================
# Shapes library.
#================================
source_group("include" FILES ${APOLLO_INCLUDE_SHAPES_GROUP})
source_group("source" FILES ${APOLLO_SOURCE_SHAPES_GROUP})

add_library(shapes ${APOLLO_INCLUDE_SHAPES_GROUP} ${APOLLO_SOURCE_SHAPES_GROUP})
target_include_directories(shapes PUBLIC ${APOLLO_SOURCE_ROOT})
target_link_libraries(shapes PUBLIC core)
set_target_properties(shapes PROPERTIES FOLDER "apollo")

#================================
# Build the tests.
#================================
if (APOLLO_BUILD_TESTS)
    FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG 5c88067bd339465513af4aec606bd2292f1b594a
        )

    find_package(Catch2 QUIET)
    if (NOT Catch2_FOUND AND NOT catch2_POPULATED)
        FetchContent_Populate(catch2)
        add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR})
        set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${catch2_SOURCE_DIR}/contrib)
    endif()

    add_subdirectory(${APOLLO_TEST_ROOT})

    #================================
    # Core tests.
    #================================
    source_group("source" FILES ${APOLLO_TEST_CORE_GROUP})
    add_executable(core_test ${APOLLO_TEST_CORE_GROUP})
    target_link_libraries(core_test PRIVATE core Catch2::Catch2)
    set_target_properties(core_test PROPERTIES FOLDER "apollo_test")

    #================================
    # Shapes tests.
    #================================
    source_group("source" FILES ${APOLLO_TEST_SHAPES_GROUP})
    add_executable(shapes_test ${APOLLO_TEST_SHAPES_GROUP})
    target_link_libraries(shapes_test PRIVATE shapes Catch2::Catch2)
    set_target_properties(shapes_test PROPERTIES FOLDER "apollo_test")

    set(APOLLO_TEST_LIST
        core_test
        shapes_test
        )

    include(CTest)
    include(Catch)
    foreach(test_target ${APOLLO_TEST_LIST})
        catch_discover_tests(${test_target})
    endforeach()
endif()
