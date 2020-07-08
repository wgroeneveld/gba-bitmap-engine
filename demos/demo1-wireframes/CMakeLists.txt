project(wireframes)

add_executable(${PROJECT_NAME}.elf
        src/main.cpp
        src/wirescene.cpp
        src/wirescene.h
        )

target_link_libraries(${PROJECT_NAME}.elf gba-bitmap-engine)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -v -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.gba
        )
