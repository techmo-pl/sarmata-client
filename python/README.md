Techmo Sarmata ASR - python client.

Stream audio to the ASR engine and prints all reponses
(status, transcription, semantic interpreatation and confidence - if applicable).


Use python 3.5 with virtual environment.
  virtualenv -p python3.5 venv
  source venv/bin/activate

Install requirements
  pip install -r requirements.txt

Run client
  python sarmata_client.py --address ip:port --grammar grammar_file.xml --wave audio.wav

Possibility to define grammar before recognitions
  python sarmata_client.py --address ip:port --define-grammar --grammar grammar_file.xml --grammar-name name
