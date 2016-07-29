grpc_root="/opt/grpc/"

$grpc_root/third_party/protobuf/src/protoc asr_service.proto --plugin=protoc-gen-grpc=$grpc_root/bins/opt/grpc_cpp_plugin --cpp_out=../libsarmata-client/

$grpc_root/third_party/protobuf/src/protoc --grpc_out=../libsarmata-client/ --plugin=protoc-gen-grpc=$grpc_root/bins/opt/grpc_cpp_plugin asr_service.proto

#protoc asr_service.proto --plugin=protoc-gen-grpc=../../third_party/grpc/vsprojects/x64/Release/grpc_cpp_plugin.exe --cpp_out=../libsarmata-client/
