project(libsarmata_client)

add_library(${PROJECT_NAME}
        sarmata_asr.grpc.pb.cc sarmata_asr.grpc.pb.h
        sarmata_asr.pb.cc sarmata_asr.pb.h
        sarmata_client.cpp sarmata_client.h
        VERSION.h
        )

# Depend on a library that we defined in the top-level file
target_link_libraries(${PROJECT_NAME}
        grpc_pkg
        )

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
