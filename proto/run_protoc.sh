protoc asr_service.proto --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin --cpp_out=../libsarmata-client/

protoc asr_service.proto --plugin=protoc-gen-grpc=../../third_party/grpc/vsprojects/x64/Release/grpc_cpp_plugin.exe --cpp_out=../libsarmata-client/
