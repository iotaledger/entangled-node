project(iota_common-node)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(${CMAKE_JS_INC})
file(GLOB SOURCE_FILES "src/interface.cpp")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

# The FetchContent module needs cmake 3.11 or above.
cmake_minimum_required(VERSION 3.11)

# fetch iota_common
include(FetchContent)
FetchContent_Declare(
  iota_common
  GIT_REPOSITORY https://github.com/iotaledger/iota_common.git
  GIT_TAG 12d54bfadead696a4ffac40ce2ba01d62e9dc35d # could be branch name, tag or commit hash.
)

if(${CMAKE_VERSION} VERSION_LESS 3.14)
    macro(FetchContent_MakeAvailable NAME)
        FetchContent_GetProperties(${NAME})
        if(NOT ${NAME}_POPULATED)
            FetchContent_Populate(${NAME})
            add_subdirectory(${${NAME}_SOURCE_DIR} ${${NAME}_BINARY_DIR})
        endif()
    endmacro()
endif()

message(STATUS "Fetching iota_common")
FetchContent_MakeAvailable(iota_common)

find_library(
    KECCAK
    NAMES keccak libkeccak
    HINTS "${CMAKE_INSTALL_PREFIX}/lib"
    NO_DEFAULT_PATH
)

# iota_common headers
target_include_directories(iota_common-node PUBLIC
  "${CMAKE_INSTALL_PREFIX}/include",
  "${CMAKE_INSTALL_PREFIX}/include/keccak",
  "${iota_common_SOURCE_DIR}"
)

# link dependent libraries
target_link_libraries(iota_common-node PUBLIC common ${KECCAK})