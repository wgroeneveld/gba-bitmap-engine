project(gba-bitmap-engine)
set_property(SOURCE
        src/gba/sin_lut.s
        src/gba/tonc_bios.s
        src/gba/tonc_memset.s
        src/background/tonc_font.s PROPERTY LANGUAGE C)
set_source_files_properties(
        src/gba/tonc_bios.s
        src/gba/tonc_memset.s
        src/background/tonc_font.s PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp")
set_source_files_properties(src/gba/tonc_bmp8.cpp PROPERTIES COMPILE_FLAGS "-Wno-pointer-arith")

add_library(${PROJECT_NAME}
        src/palette/palette_manager.cpp
        src/palette/combined_palette.cpp
        src/background/text_stream.cpp
        src/gba/sin_lut.s
        src/gba/tonc_memset.s
        src/gba/tonc_bios.s
        src/gba/tonc_bmp8.cpp
        src/background/tonc_font.s
        src/renderer/gba_engine.cpp
        src/renderer/pixelrenderer.cpp
        src/renderer/wirerenderer.cpp
        src/math.cpp
        src/sound_control.cpp src/scene.cpp src/timer.cpp src/vectorfx.cpp src/mesh.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE src)

install(TARGETS ${PROJECT_NAME} EXPORT GbaSpriteEngineTargets
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
