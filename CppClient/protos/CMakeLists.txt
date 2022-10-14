project(protos)
set(CMAKE_VERBOSE_MAKEFILE ON)

message(STATUS "Name of project ${PROJECT_NAME}")

find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using Protobuf ${protobuf_VERSION}")

find_package(gRPC CONFIG REQUIRED)
message (STATUS "Using gRPC ${gRPC_VERSION}")

set(SRC_FILES
    stock_messages.proto
    stock_service.proto
)
add_library(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CONAN_INCLUDE_DIRS}
)

target_link_directories(${PROJECT_NAME} 
    PUBLIC
        ${CONAN_LIB_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        ${CONAN_LIBS}
)

# auto generate the grpc files
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET ${PROJECT_NAME} LANGUAGE cpp PROTOS ${SRC_FILES})
protobuf_generate(TARGET ${PROJECT_NAME} LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}" PLUGIN_OPTIONS "generate_mock_code=true")
