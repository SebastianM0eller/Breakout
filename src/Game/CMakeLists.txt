add_executable(Game main.cpp)

target_link_libraries(Game PRIVATE Engine)

target_include_directories(Game PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_sources(Game PRIVATE
  source/BreakoutGameLayer.cpp
  source/Systems/CollisionSystem.cpp
)


set(ASSET_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")
set(ASSET_DEST_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets")

add_custom_command(
  TARGET Game POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "${ASSET_DEST_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${ASSET_SOURCE_DIR}"
      "${ASSET_DEST_DIR}"

)

if(MSVC)
  target_compile_options(Game PRIVATE /W4 /WX)
else()
  target_compile_options(Game PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
