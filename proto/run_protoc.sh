protoc asr_service.proto --plugin=protoc-gen-grpc=/opt/grpc/bins/opt/grpc_cpp_plugin --cpp_out=../libsarmata-client/

#protoc --grpc_out=../libsarmata-client/ --plugin=protoc-gen-grpc=../../third_party/grpc/vsprojects/x64/Release/grpc_cpp_plugin.exe asr_service.proto
#protoc asr_service.proto --plugin=protoc-gen-grpc=../../third_party/grpc/vsprojects/x64/Release/grpc_cpp_plugin.exe --cpp_out=../libsarmata-client/
