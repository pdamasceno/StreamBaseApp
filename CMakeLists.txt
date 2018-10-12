cmake_minimum_required(VERSION 3.10)
project(StreamBaseApp)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

include_directories(./include)
include_directories(./include/collection)
include_directories(./include/communication)

set(StreamBaseServer_SourceFiles include/Server.h include/collection/GameGenerator.h src/collection/GameGenerator.cpp include/collection/GameGenre.h src/collection/GameGenre.cpp include/communication/Header.h src/communication/Header.cpp src/collection/Shooter.cpp include/communication/CommonMessage.h src/communication/CommonMessage.cpp include/collection/RPG.h src/collection/RPG.cpp include/collection/Racing.h src/collection/Racing.cpp include/communication/Writer.h src/communication/Writer.cpp include/communication/Reader.h src/communication/Reader.cpp include/communication/Command.h src/communication/Command.cpp)
add_executable(Server src/Server.cpp ${StreamBaseServer_SourceFiles})

set(StreamBaseClient_SourceFiles include/Client.h include/communication/Header.h src/communication/Header.cpp include/communication/CommonMessage.h src/communication/CommonMessage.cpp include/communication/Writer.h src/communication/Writer.cpp include/communication/Reader.h src/communication/Reader.cpp include/communication/Command.h src/communication/Command.cpp)
add_executable(Client src/Client.cpp ${StreamBaseClient_SourceFiles})
