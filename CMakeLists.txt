cmake_minimum_required(VERSION 2.8)
project(MGC)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)

# includes cmake/FindSDL2.cmake
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
link_libraries(${SDL2_LIBRARY})
#MESSAGE( WARNING "SDL2_INCLUDE_DIR:         " ${SDL2_INCLUDE_DIR} )
#MESSAGE( WARNING "SDL2_LIBRARY:         " ${SDL2_LIBRARY} )

find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIR})
link_libraries(${SDL2_TTF_LIBRARIES})
#MESSAGE( WARNING "SDL2_TTF_INCLUDE_DIR:         " ${SDL2_TTF_INCLUDE_DIR} )
#MESSAGE( WARNING "SDL2_TTF_LIBRARIES:         " ${SDL2_TTF_LIBRARIES} )

find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
link_libraries(${SDL2_IMAGE_LIBRARIES})
#MESSAGE( WARNING "SDL2_IMAGE_INCLUDE_DIR:         " ${SDL2_IMAGE_INCLUDE_DIR} )
#MESSAGE( WARNING "SDL2_IMAGE_LIBRARIES:         " ${SDL2_IMAGE_LIBRARIES} )

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
link_libraries(${OPENGL_LIBRARIES})
#MESSAGE( WARNING "OPENGL_INCLUDE_DIR:         " ${OPENGL_INCLUDE_DIR} )
#MESSAGE( WARNING "OPENGL_LIBRARIES:         " ${OPENGL_LIBRARIES} )

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

find_package(Lua REQUIRED)
include_directories(${LUA_INCLUDE_DIR})
link_libraries(${LUA_LIBRARIES})
MESSAGE( WARNING "LUA_INCLUDE_DIR:         " ${LUA_INCLUDE_DIR} )
MESSAGE( WARNING "LUA_LIBRARIES:         " ${LUA_LIBRARIES} )

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})
link_libraries(${FREETYPE_LIBRARIES})
MESSAGE( WARNING "FREETYPE_INCLUDE_DIRS:         " ${FREETYPE_INCLUDE_DIRS} )
MESSAGE( WARNING "FREETYPE_LIBRARIES:         " ${FREETYPE_LIBRARIES} )


include_directories(include)

file(GLOB SOURCE_FILES
        "*.cpp"
        "*.h"
        )


# set(SOURCE_FILES ${SOURCE_FILES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})


add_custom_command(TARGET MGC PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:MGC>/Resources)
