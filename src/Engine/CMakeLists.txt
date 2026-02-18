set(ENGINE_SOURCES
  source/Application.cpp
  source/ResourceManager.cpp
)

set(ENGINE_HEADERS
  include/Engine/Application.h
  include/Engine/ResourceManager.h
  include/Engine/Renderer.h
)

add_library(Engine STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})

target_include_directories(Engine
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"

        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/source"
)

target_link_libraries(Engine
    PUBLIC
        SFML::Graphics 
        SFML::Audio 
        SFML::Window 
        SFML::System
)

if(MSVC)
  target_compile_options(Engine PRIVATE /W4 /WX)
else()
  target_compile_options(Engine PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
