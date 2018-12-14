C++ implementation of Sarmata ASR gRPC client.

Contents:
- `libsarmata-client`     Library implementing communication with Techmo Sarmata ASR gRPC service.
- `sarmata-client`        Example of program using the library.

This project uses cmake build.

Dependencies are:  
- Boost     provided as `boost_pkg`  
    Default location: `/opt/boost_1.60.0`  
    If not installed, from parent directory run `sudo ./tools/install_boost.sh`  
- gRPC      provided as `grpc_pkg`  
    Default location: `/opt/grpc_v1.7.2`  
    If not installed, from parent directory run `sudo ./tools/install_grpc.sh`  
- OpenSSL   provided as `ssl_pkg`  
- DL        provided as `dl_pkg`  

To regenerate sources from `.proto`, run:
```
./make_proto.sh
```
This might be required when using other gRPC or Protocol Buffers version.

Build:
```
mkdir build && cd build && cmake .. && make -j 4
```

Run:
```
./build/sarmata_client --service-address 192.168.1.1:4321 --wave-path /path/to/audio.wav --grammar /path/to/grammar.xml
```

There is a possibility to define grammar before recognitions:
```
./build/sarmata_client --service-address 192.168.1.1:4321 --define-grammar --grammar /path/to/grammar.xml --grammar-name simple_grammar
```
and then run recognition:
```
./build/sarmata_client --service-address 192.168.1.1:4321 --grammar-name simple_grammar --wave-path /path/to/audio.wav
```

To delete a cached grammar, run with `--define-grammar` and `--grammar-name` options only (no `--grammar` option):
```
./build/sarmata_client --service-address 192.168.1.1:4321 --define-grammar --grammar-name grammar_to_delete
```

Options:
```
  --help                          Print help message.
  --service-address arg           IP address and port (address:port) of a 
                                  service the client will connect to.
  --wave-path arg                 Path to wave file with audio content to be 
                                  sent to service via RPC.
  --grammar-name arg              Name (ID) of the grammar in the service's 
                                  grammar cache.
  --grammar arg                   SRGS grammar file (ABNF or XML format 
                                  accepted).
  --session-id arg                Session ID to be passed to the service. If 
                                  not specified, the service will generate a 
                                  default session ID itself.
  --grpc-timeout arg (=0)         Timeout in milliseconds used to set gRPC 
                                  deadline - how long the client is willing to 
                                  wait for a reply from the server. If not 
                                  specified, the service will set the deadline 
                                  to a very large number.
  --service-settings arg          Semicolon-separated list of key=value pairs 
                                  defining settings to be sent to service via 
                                  gRPC request.
  --max-alternatives arg (=1)     Maximum number of recognition hypotheses to 
                                  be returned.
  --no-match-threshold arg        Confidence acceptance threshold.
  --no-input-timeout arg          MRCPv2 No-Input-Timeout in milliseconds.
  --recognition-timeout arg       MRCPv2 Recognition-Timeout in milliseconds.
  --speech-complete-timeout arg   MRCPv2 Speech-Complete-Timeout in 
                                  milliseconds.
  --speech-incomplete-timeout arg MRCPv2 Speech-Incomplete-Timeout in 
                                  milliseconds.
  --define-grammar                If present, will perform DefineGrammar call 
                                  for given --grammar-name and --grammar (will 
                                  not call Recognize).
```
