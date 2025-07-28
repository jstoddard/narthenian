cmake_minimum_required(VERSION 3.31)
project(narthenian)

set(CMAKE_CXX_STANDARD 20)

#For downloading SDL3 - Thanks, Erik Yuzwa on YouTube
include(FetchContent)

function(include_dependency libName gitURL gitTag)
    FetchContent_Declare(${libName}
            GIT_REPOSITORY  ${gitURL}
            GIT_TAG         ${gitTag}
            GIT_SHALLOW     TRUE
            GIT_PROGRESS    TRUE
    )
    FetchContent_MakeAvailable(${libName})
endfunction()

find_package(SDL3 QUIET)
if (NOT SDL3_FOUND)
    include_dependency(SDL3 https://github.com/libsdl-org/SDL.git release-3.2.16)
endif()

find_package(SDL3_image QUIET)
if (NOT SDL3_image_FOUND)
    include_dependency(SDL3_image https://github.com/libsdl-org/SDL_image.git release-3.2.4)
endif()

add_executable(narthenian main.cpp
        AreaMap.cpp
        AreaMap.h
        Screen.h
        GameScreen.cpp
        GameScreen.h
        Screen.cpp
        MessageBox.cpp
        MessageBox.h
        MeleeScreen.cpp
        MeleeScreen.h
        Combatant.cpp
        Combatant.h
        Menu.cpp
        Menu.h
        Sound.cpp
        Sound.h
        DialogScreen.cpp
        DialogScreen.h
        Player.cpp
        Player.h
        common.h
        NPC.cpp
        NPC.h
        Innkeeper.cpp
        Innkeeper.h
        InnScreen.cpp
        InnScreen.h)
target_include_directories(narthenian PUBLIC ${SDL3_INCLUDE_DIRS} ${SDL3_image_INCLUDE_DIRS})
target_link_libraries(narthenian PRIVATE SDL3_image::SDL3_image SDL3::SDL3)
