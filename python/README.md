Techmo Sarmata ASR - python client.

Stream audio to the ASR engine and prints all reponses:
 - status, 
 - transcription, 
 - semantic interpreatation, 
 - confidence,
(when applicable).

To run:
 - Use python 3.5 with virtual environment and install required packages.
```
  virtualenv -p python3.5 venv
  
  source venv/bin/activate
  
  pip install -r requirements.txt
```

Dependencies (listed in requirements.txt):
 - grpcio
 - grpcio-tools
 - protobuf
 - pydub
  
To regenerate sources from `.proto`, run:
```
./make_proto.sh
```
This might be required when using other gRPC or Protocol Buffers version.


**Run:**
```
  python sarmata_client.py --address ip:port --grammar grammar_file.xml --wave audio.wav
```
 
There is a possibility to define grammar before recognitions:
```
  python sarmata_client.py --address ip:port --define-grammar --grammar grammar_file.xml --grammar-name simple_grammar
```
and then run recognition:
```
  python sarmata_client.py --address ip:port --grammar-name simple_grammar --wave audio.wav
```