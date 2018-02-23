# API Documentation
<a name="top"/>

### Table of Contents

- [sarmata_asr.proto](#sarmata_asr.proto)
    - [ASR](#techmo.sarmata.ASR)
    - [ConfigField](#techmo.sarmata.ConfigField)
    - [DefineGrammarRequest](#techmo.sarmata.DefineGrammarRequest)
    - [DefineGrammarResponse](#techmo.sarmata.DefineGrammarResponse)
    - [RecognitionConfig](#techmo.sarmata.RecognitionConfig)
    - [RecognizeRequest](#techmo.sarmata.RecognizeRequest)
    - [RecognizeResponse](#techmo.sarmata.RecognizeResponse)
    - [RecognizedPhrase](#techmo.sarmata.RecognizedPhrase)
    - [RecognizedPhrase.RecognizedWord](#techmo.sarmata.RecognizedPhrase.RecognizedWord)
    - [ResponseStatus](#techmo.sarmata.ResponseStatus)
  
- [Scalar Value Types](#scalar-value-types)



<a name="sarmata_asr.proto"/>
<p align="right"><a href="#top">Top</a></p>

## sarmata_asr.proto
Techmo Sarmata ASR API
version: 2.0.0
authors: Dawid Skurzok, Paweł Jaciów
date:    2018-01-30

Some content is derived from:
https://github.com/googleapis/googleapis/blob/master/google/cloud/speech/v1/cloud_speech.proto


<a name="techmo.sarmata.ASR"/>

### ASR
Service that implements Techmo Automatic-Speech-Recognition (ASR) API.

Grammar persistence options:
- per alive connection: user opens special channel to service to define grammars, grammars are kept as long as connection is alive,
- predefined per user on disk: service keep predefined grammar as files on disk, loads it at startup,
- auto-cache: all grammars are cached automatically using its hash as ID, max number of cached grammars and prune policy must be defined.

Users account manipulation will be provided by another service.

| Method Name | Request Type | Response Type | Description |
| ----------- | ------------ | ------------- | ------------|
| Recognize | [RecognizeRequest](#techmo.sarmata.RecognizeRequest) | [RecognizeResponse](#techmo.sarmata.RecognizeRequest) | Performs bidirectional streaming speech recognition using given grammar. Receive results while sending audio. |
| DefineGrammar | [DefineGrammarRequest](#techmo.sarmata.DefineGrammarRequest) | [DefineGrammarResponse](#techmo.sarmata.DefineGrammarRequest) | Defines grammar that will be stored for future use in recognitions requested by `Recognize`. |

 <!-- end services -->


<a name="techmo.sarmata.ConfigField"/>

### ConfigField
Provides a pair of configuration field name and value.


| Field | Type | Description |
| ----- | ---- | ----------- |
| key | [string](#string) | Name of configuration field. |
| value | [string](#string) | Value of configuration field. |






<a name="techmo.sarmata.DefineGrammarRequest"/>

### DefineGrammarRequest
The top-level message sent by the client for the `DefineGrammar` method.
It will define grammar specified in `grammar` field for use in future recognitions
under ID given by `name` field.
When empty `grammar` is sent, the `name` grammar will be deleted from cache.


| Field | Type | Description |
| ----- | ---- | ----------- |
| token | [string](#string) | [*Optional*] Authorization token. |
| name | [string](#string) | [*Required*] Grammar ID to store the grammar for future use with. |
| grammar | [string](#string) | [*Optional*] Grammar data to be stored for future use. Supported grammar formats are XML and ABNF as specified by [W3C](https://www.w3.org/TR/speech-grammar/). Empty (not set) requests deletion of the grammar cached with ID `name`. |






<a name="techmo.sarmata.DefineGrammarResponse"/>

### DefineGrammarResponse
The only message returned to the client by the `DefineGrammar` method. It
contains a grammar creation confirmation or an error message.


| Field | Type | Description |
| ----- | ---- | ----------- |
| status | [ResponseStatus](#techmo.sarmata.ResponseStatus) | Status and type of message. |
| error | [string](#string) | Error message. |
| ok | [bool](#bool) | True if grammar was created. |






<a name="techmo.sarmata.RecognitionConfig"/>

### RecognitionConfig
Provides information to the recognizer that specifies
how to process the request.


| Field | Type | Description |
| ----- | ---- | ----------- |
| config | [ConfigField](#techmo.sarmata.ConfigField) | [*Optional*] A means to provide additional configuration fields via request. |
| token | [string](#string) | [*Optional*] Authorization token. |
| sample_rate_hertz | [int32](#int32) | [*Required*] Sample rate in Hertz of the audio data sent in all `RecognizeRequest` messages. |
| max_alternatives | [int32](#int32) | [*Optional*] Maximum number of recognition hypotheses to be returned. Specifically, the maximum number of `Phrase` messages within each `RecognizeResponse`. The server may return fewer than `max_alternatives`. If omitted, will return a maximum of one. |
| name | [string](#string) | Grammar ID of a stored grammar to use for the recognition. |
| data | [string](#string) | Grammar data to use for the recognition. |






<a name="techmo.sarmata.RecognizeRequest"/>

### RecognizeRequest
The top-level message sent by the client for the `Recognize` method.
Multiple `RecognizeRequest` messages are sent. The first message
must contain a `config` message and must not contain `audio` data.
All subsequent messages must contain `audio` data and must not contain a
`config` message.


| Field | Type | Description |
| ----- | ---- | ----------- |
| config | [RecognitionConfig](#techmo.sarmata.RecognitionConfig) | The `config` message provides information to the recognizer that specifies how to process the request. |
| audio_content | [bytes](#bytes) | The audio data to be recognized. Sequential chunks of audio data may be sent in sequential `RecognizeRequest` messages or whole data may be send in one message. |






<a name="techmo.sarmata.RecognizeResponse"/>

### RecognizeResponse
`RecognizeResponse` is the only message returned to the client by
`Recognize`. A series of one or more `RecognizeResponse`
messages are streamed back to the client.


| Field | Type | Description |
| ----- | ---- | ----------- |
| status | [ResponseStatus](#techmo.sarmata.ResponseStatus) | Status and type of message. |
| error | [string](#string) | Critical error message. |
| warning | [string](#string) | Minor error message, e.g. error in transcription. |
| event_time | [int32](#int32) | Time of event (if applicable). |
| results | [RecognizedPhrase](#techmo.sarmata.RecognizedPhrase) | List of recognized phrases ordered by probability. |






<a name="techmo.sarmata.RecognizedPhrase"/>

### RecognizedPhrase
A single phrase recognition result.


| Field | Type | Description |
| ----- | ---- | ----------- |
| words | [RecognizedPhrase.RecognizedWord](#techmo.sarmata.RecognizedPhrase.RecognizedWord) | Words in recognized phrase. |
| confidence | [double](#double) | Confidence of phrase recognition. |
| correct | [bool](#bool) | Is recognition considered correct (confidence above thresshold). |
| semantic_interpretation | [string](#string) | Semantic Interpretation (in JSON). |






<a name="techmo.sarmata.RecognizedPhrase.RecognizedWord"/>

### RecognizedPhrase.RecognizedWord
A single word recognition result.


| Field | Type | Description |
| ----- | ---- | ----------- |
| transcript | [string](#string) | Recognized word transcription. |
| confidence | [double](#double) | Word recognition confidence. |
| start | [int32](#int32) | Recognized word begin time. |
| end | [int32](#int32) | Recognized word end time. |
| logprob | [double](#double) | Logarithmic probability of the recognized word. |





 <!-- end messages -->


<a name="techmo.sarmata.ResponseStatus"/>

### ResponseStatus
Indicates the status and type of message.

| Name | Number | Description |
| ---- | ------ | ----------- |
| EMPTY | 0 | Should not be set. |
| SUCCESS | 1 | Phrase recognized. |
| PARTIAL_MATCH | 2 | Recognized only beginning of the utterence. |
| NO_MATCH | 3 | No pharse recognized. |
| NO_INPUT_TIMEOUT | 4 | No spech or no match in expected time. |
| GRAMMAR_LOAD_FAILURE | 5 | Grammar connot be loaded. Details in `error` field. |
| GRAMMAR_COMPILATION_FAILURE | 6 | Grammar compilation error. Details in `error` field. |
| RECOGNIZER_ERROR | 7 | Iternal error. |
| TOO_MUCH_SPEECH_TIMEOUT | 8 | Speech to long. |
| CANCELLED | 9 | Recognition cancelled. |
| START_OF_INPUT | 10 | Start of speech detected. |
| END_OF_AUDIO | 11 | Finished processing. |
| SEMANTICS_FAILURE | 12 | Error in Semantic Interpretation. |


 <!-- end enums -->

 <!-- end HasExtensions -->



<p align="right"><a href="#top">Top</a></p>
## Scalar Value Types

| .proto Type | Notes | C++ Type | Java Type | Python Type |
| ----------- | ----- | -------- | --------- | ----------- |
| <a name="double" /> double |  | double | double | float |
| <a name="float" /> float |  | float | float | float |
| <a name="int32" /> int32 | Uses variable-length encoding. Inefficient for encoding negative numbers – if your field is likely to have negative values, use sint32 instead. | int32 | int | int |
| <a name="int64" /> int64 | Uses variable-length encoding. Inefficient for encoding negative numbers – if your field is likely to have negative values, use sint64 instead. | int64 | long | int/long |
| <a name="uint32" /> uint32 | Uses variable-length encoding. | uint32 | int | int/long |
| <a name="uint64" /> uint64 | Uses variable-length encoding. | uint64 | long | int/long |
| <a name="sint32" /> sint32 | Uses variable-length encoding. Signed int value. These more efficiently encode negative numbers than regular int32s. | int32 | int | int |
| <a name="sint64" /> sint64 | Uses variable-length encoding. Signed int value. These more efficiently encode negative numbers than regular int64s. | int64 | long | int/long |
| <a name="fixed32" /> fixed32 | Always four bytes. More efficient than uint32 if values are often greater than 2^28. | uint32 | int | int |
| <a name="fixed64" /> fixed64 | Always eight bytes. More efficient than uint64 if values are often greater than 2^56. | uint64 | long | int/long |
| <a name="sfixed32" /> sfixed32 | Always four bytes. | int32 | int | int |
| <a name="sfixed64" /> sfixed64 | Always eight bytes. | int64 | long | int/long |
| <a name="bool" /> bool |  | bool | boolean | boolean |
| <a name="string" /> string | A string must always contain UTF-8 encoded or 7-bit ASCII text. | string | String | str/unicode |
| <a name="bytes" /> bytes | May contain any arbitrary sequence of bytes. | string | ByteString | str |

