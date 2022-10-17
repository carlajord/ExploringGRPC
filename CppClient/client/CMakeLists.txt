project(client LANGUAGES CXX)

find_package(Threads)

set (SOURCES
    stockclient.h
    stockclient.cpp
    threadmanager.h
    threadmanager.cpp
    main.cpp
    )

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})


add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        protos
)

target_compile_definitions(${PROJECT_NAME} PUBLIC DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data/")
