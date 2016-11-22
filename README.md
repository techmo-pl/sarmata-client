# sarmata-client
Client to sarmata ASR grpc service

Build on grpc v1.0 : https://github.com/grpc/grpc/tree/v1.0.x

The following configuration options are supported:

 - sampling-rate (=16 000) - sampling rate of the input audio signal [Hz]
 
 - complete-timeout (=500) - length of silence (in miliseconds) required following user speech before the speech recognizer finalizes a result, either accepting (MATCH) or rejecting (NO_MATCH) [ms]
 
 
 - incomplete-timeout arg (=2 000) - specifies the required length of silence (in miliseconds) following user speech after which a recognizer finalizes a result with PARTIAL_MATCH event. 
 The speech prior to the silence is an incomplete match to the active grammar [ms]
   
 - no-input-timeout (=3 000) - specifies the period of time (in miliseconds) when recognition is started and there is no speech detected before recognizer terminates the recognition with NO_INPUT event [ms]
 
 - no-rec-timeout (=5 000) - specifies the period of time (in miliseconds) when recognition is started, speech is detected, but there is no match to the grammar. Recognizer terminates the recognition operation with TOO_MUCH_SPEECH_TIMEOUT event[ms]
 
 
 - nbest (=20) - the maximum number of alternatives returned by recognizer that are matched to grammar
