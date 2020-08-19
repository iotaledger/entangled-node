project(entangled-node)
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
  GIT_TAG 1b56a5282933fb674181001630e7b2e2c33b5eea # could be branch name, tag or commit hash.
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

# iota_common headers
target_include_directories(entangled-node PUBLIC
  "${CMAKE_INSTALL_PREFIX}/include",
  "${iota_common_SOURCE_DIR}"
)

# link dependent libraries
target_link_libraries(entangled-node PUBLIC
  logger
  keccak
)