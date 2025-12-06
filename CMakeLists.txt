cmake_minimum_required(VERSION 4.0)
project(Kasyno)

set(CMAKE_CXX_STANDARD 20)

if(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

# Główna aplikacja (BEZ TestPlayground.cpp)
add_executable(Kasyno
        main.cpp
        Player.cpp
        RoundUI.cpp
        Casino.cpp
        Rng.cpp
        FileHandler.cpp
        Games/SlotsGame.cpp
        Games/SlotsGame.h
        Games/RouletteGame.cpp
        Games/RouletteGame.h
        Games/BlackjackGame.cpp
        Games/BlackjackGame.h
        Games/RouletteTypes.h
)

# Testy (BEZ main.cpp)
add_executable(KasynoTests
        TestPlayground.cpp
        Player.cpp
        RoundUI.cpp
        Casino.cpp
        Rng.cpp
        FileHandler.cpp
)