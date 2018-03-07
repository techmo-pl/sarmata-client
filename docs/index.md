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
    - [TimeoutSettings](#techmo.sarmata.TimeoutSettings)
    - [ResponseStatus](#techmo.sarmata.ResponseStatus)
  
- [Scalar Value Types](#scalar-value-types)



<a name="sarmata_asr.proto"/>
<p align="right"><a href="#top">Top</a></p>

## sarmata_asr.proto
Techmo Sarmata ASR API
version: 2.0.0
authors: Dawid Skurzok, Paweł Jaciów
date:    2018-02-26

Some content is derived from:
https://github.com/googleapis/googleapis/blob/master/google/cloud/speech/v1/cloud_speech.proto


<a name="techmo.sarmata.ASR"/>

### ASR
Service that implements Techmo Automatic-Speech-Recognition (ASR) API.

Grammar persistence options:
- per alive connection: user opens special channel to service to define grammars, grammars are kept as long as connection is alive,
- predefined per user on disk: service keep predefined grammar as files on disk, loads it at startup,
- auto-cache: all grammars are cached automatically using its hash as ID, max number of cached grammars and prune policy must be defined.

Supported grammar formats are XML and ABNF as specified by [W3C SRGS](https://www.w3.org/TR/speech-grammar/).

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
It will define grammar specified in `grammar_data` field for use in future recognitions
under ID given by `grammar_name` field.
When empty `grammar_data` is sent, the `grammar_name` grammar will be deleted from cache.


| Field | Type | Description |
| ----- | ---- | ----------- |
| grammar_name | [string](#string) | [*Required*] Grammar ID to store the grammar for future use with. |
| grammar_data | [string](#string) | [*Optional*] Grammar data to be stored for future use. Supported grammar formats are XML and ABNF as specified by [W3C SRGS](https://www.w3.org/TR/speech-grammar/). Empty (not set) requests deletion of the grammar cached with ID `grammar_name`. |






<a name="techmo.sarmata.DefineGrammarResponse"/>

### DefineGrammarResponse
The only message returned to the client by the `DefineGrammar` method. It
contains a grammar creation confirmation or an error message.


| Field | Type | Description |
| ----- | ---- | ----------- |
| status | [ResponseStatus](#techmo.sarmata.ResponseStatus) | Status and type of message. |
| error | [string](#string) | Error message. Set only when an error occurred. |
| ok | [bool](#bool) | True if grammar was created. |






<a name="techmo.sarmata.RecognitionConfig"/>

### RecognitionConfig
Provides information to the recognizer that specifies
how to process the request.

The `RecognitionConfig` message must provide sample rate
in `sample_rate_hertz` field and a grammar as only one of `grammar_name`
or `grammar_data` fields.
Other fields are optional to set.


| Field | Type | Description |
| ----- | ---- | ----------- |
| grammar_name | [string](#string) | [*One-of*][*Required*] ID of a stored grammar to use for the recognition. |
| grammar_data | [string](#string) | [*One-of*][*Required*] Grammar data to use for the recognition. Supported grammar formats are XML and ABNF as specified by [W3C SRGS](https://www.w3.org/TR/speech-grammar/). |
| sample_rate_hertz | [int32](#int32) | [*Required*] Sample rate in hertz of the audio data sent in all `RecognizeRequest` messages. |
| max_alternatives | [int32](#int32) | [*Optional*] Maximum number of recognition hypotheses to be returned. Specifically, the maximum number of `Phrase` messages within each `RecognizeResponse`. The server may return fewer than `max_alternatives`. If omitted, will return a maximum of one. |
| no_match_threshold | [double](#double) | [*Optional*] Confidence threshold to decide if phrase is correct. Results with score below the threshold are considered no match. |
| timeout_settings | [TimeoutSettings](#techmo.sarmata.TimeoutSettings) | [*Optional*] MRCPv2-related timeout settings. Recognition will be interrupted when any of the timeouts is met. |
| additional_settings | [ConfigField](#techmo.sarmata.ConfigField) | [*Optional*] A means to provide additional configuration via request. |

Additional configuration can be set via `additional_settings` field.
The following parameters are allowed to be set that way:

| Parameter | Type | Default value | Description |
| --------- |:----:|:-------------:| ----------- |
| *beam-width* | `int` | 20 | With of the beam in log prob. |
| *speech-start* | `int` | 200 | Time of continuous speech before speech start event is passed [ms]. |
| *order-by-confidence* | `bool` | false | Order phrases by confidence instead of likelihood. |
| *keep-sil* | `bool` | false | Keep silence path even if it should be pruned or does not match grammar. |
| *enable-incomplete-match* | `bool` | true | Enable incomplete match. |
| *enable-incomplete-match-parse* | `bool` | false | Enable incomplete match. |
| *garbage-beam* | `int` | 5 | Beam on garbage model. |
| *show-sil* | `bool` | false | Show silence in recognitions. |
| *show-garbage* | `bool` | false | Show garbage in recognitions. |
| *flush-if-no-rec* | `bool` | true | Force recognition on end of stream. |
| *enable-garbage* | `bool` | false | Enable garbage model. |
| *use-vad* | `bool` | false | Use Voice Activity Detector. |
| *vad-sil-ll* | `double` | 1.0 | Likelihood assigned to silent frames by VAD. |
| *vad-nonsil-ll* | `double` | 0.0 | Likelihood assigned to non-silent frames by VAD. |






<a name="techmo.sarmata.RecognizeRequest"/>

### RecognizeRequest
The top-level message sent by the client for the `Recognize` method.
Multiple `RecognizeRequest` messages are sent. The first message
must contain a `config` message and must not contain `audio` data.
All subsequent messages must contain `audio` data and must not contain a
`config` message.


| Field | Type | Description |
| ----- | ---- | ----------- |
| config | [RecognitionConfig](#techmo.sarmata.RecognitionConfig) | [*One-of*] The `config` message provides information to the recognizer that specifies how to process the request. |
| audio_content | [bytes](#bytes) | [*One-of*] The audio data to be recognized. Sequential chunks of audio data may be sent in sequential `RecognizeRequest` messages or whole data may be send in one message. |






<a name="techmo.sarmata.RecognizeResponse"/>

### RecognizeResponse
`RecognizeResponse` is the only message returned to the client by
`Recognize`. A series of one or more `RecognizeResponse`
messages are streamed back to the client.


| Field | Type | Description |
| ----- | ---- | ----------- |
| status | [ResponseStatus](#techmo.sarmata.ResponseStatus) | Status and type of the message. Client should check statuses of received responses and stop sending audio data when received any of the following ones: - GRAMMAR_LOAD_FAILURE, - GRAMMAR_COMPILATION_FAILURE, - RECOGNIZER_ERROR, - SEMANTICS_FAILURE, which means that the service encountered an error (details in `error` field) and stopped processing incoming audio. |
| error | [string](#string) | Status-specific error message. Is set only for statuses listed in `status` field's description as meaning errors. |
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






<a name="techmo.sarmata.TimeoutSettings"/>

### TimeoutSettings
MRCPv2-related timeout settings. The values are in milliseconds.


| Field | Type | Description |
| ----- | ---- | ----------- |
| no_input_timeout | [int32](#int32) | [*Optional*] [No-Input-Timeout](https://tools.ietf.org/html/rfc6787#section-9.4.6) When recognition is started and there is no speech detected for a certain period of time, the recognizer can send a RECOGNITION- COMPLETE event to the client with a Completion-Cause of "no-input- timeout" and terminate the recognition operation. |
| recognition_timeout | [int32](#int32) | [*Optional*] [Recognition-Timeout](https://tools.ietf.org/html/rfc6787#section-9.4.7) When recognition is started and there is no match for a certain period of time, the recognizer can send a RECOGNITION-COMPLETE event to the client and terminate the recognition operation. |
| speech_complete_timeout | [int32](#int32) | [*Optional*] [Speech-Complete-Timeout](https://tools.ietf.org/html/rfc6787#section-9.4.15) Specifies the length of silence required following user speech before the speech recognizer finalizes a result (either accepting it or generating a no-match result). The Speech-Complete- Timeout value applies when the recognizer currently has a complete match against an active grammar, and specifies how long the recognizer MUST wait for more input before declaring a match. By contrast, the Speech-Incomplete-Timeout is used when the speech is an incomplete match to an active grammar. A long Speech-Complete-Timeout value delays the result to the client and therefore makes the application's response to a user slow. A short Speech-Complete-Timeout may lead to an utterance being broken up inappropriately. Reasonable speech complete timeout values are typically in the range of 300 milliseconds to 1000 milliseconds. |
| speech_incomplete_timeout | [int32](#int32) | [*Optional*] [Speech-Incomplete-Timeout](https://tools.ietf.org/html/rfc6787#section-9.4.16) Specifies the required length of silence following user speech after which a recognizer finalizes a result. The incomplete timeout applies when the speech prior to the silence is an incomplete match of all active grammars. In this case, once the timeout is triggered, the partial result is rejected (with a Completion-Cause of "partial-match"). The Speech-Incomplete-Timeout also applies when the speech prior to the silence is a complete match of an active grammar, but where it is possible to speak further and still match the grammar. By contrast, the Speech-Complete-Timeout is used when the speech is a complete match to an active grammar and no further spoken words can continue to represent a match. A long Speech-Incomplete-Timeout value delays the result to the client and therefore makes the application's response to a user slow. A short Speech-Incomplete-Timeout may lead to an utterance being broken up inappropriately. The Speech-Incomplete-Timeout is usually longer than the Speech- Complete-Timeout to allow users to pause mid-utterance (for example, to breathe). |





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
| GRAMMAR_LOAD_FAILURE | 5 | Grammar connot be loaded. |
| GRAMMAR_COMPILATION_FAILURE | 6 | Grammar compilation error. |
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

