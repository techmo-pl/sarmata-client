
requires:

- grpc - Google's Remote Procedure Call library: https://github.com/grpc/grpc/tree/v1.0.x -  installed in /opt/grpc (path can be changed in SConstruct file)
- scons
- c++11 compatible compiler


To regenerate sources from `.proto`, run:
```
./make_proto.sh
```
This might be required when using other gRPC or Protocol Buffers version.


build command:
```
scons
```

to run test program:
```
sarmata-client/sarmata-client ADDRESS:PORT audio.wav grammar.txt
```
