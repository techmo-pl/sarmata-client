Techmo Sarmata ASR - python client.

Stream audio from wave or microphone to the ASR engine and prints all reponses:
 - status, 
 - transcription, 
 - semantic interpretation, 
 - confidence,
(when applicable).

Dependencies (listed in requirements.txt):
 - grpcio
 - grpcio-tools
 - protobuf
 - pydub
 - pyaudio

`pyaudio` requires development packages of Python and PortAudio:
```
sudo apt-get install python3.5-dev portaudio19-dev
```

To regenerate sources from `.proto`, run (virtualenv must be enabled before):
```
./make_proto.sh
```
This might be required when using other gRPC or Protocol Buffers version.

To run:
 - Use python 3.5 with virtual environment and install required packages.
```
virtualenv -p python3.5 venv
source venv/bin/activate
pip install -r requirements.txt
```


**Run (wave):**
```
python sarmata_client.py --address ip:port --grammar grammar_file.xml --wave audio.wav
```
 
**Run (microphone):**
```
python sarmata_client.py --address ip:port --grammar grammar_file.xml --mic
```
 
There is a possibility to define grammar before recognitions:
```
python sarmata_client.py --address ip:port --define-grammar --grammar grammar_file.xml --grammar-name simple_grammar
```
and then run recognition:
```
python sarmata_client.py --address ip:port --grammar-name simple_grammar --wave audio.wav
```
or
```
python sarmata_client.py --address ip:port --grammar-name simple_grammar --mic
```
