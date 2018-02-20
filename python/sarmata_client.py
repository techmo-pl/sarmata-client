#!/usr/bin/env python
# -*- coding: utf-8 -*-

from argparse import ArgumentParser
from service.sarmata_settings import SarmataSettings
from service.sarmata_recognize import SarmataRecognizer
from service.sarmata_asr_pb2 import ResponseStatus, EMPTY, START_OF_INPUT
from utils.audio_source import AudioStream
from utils.mic_source import MicrophoneStream
import sys
from VERSION import SARMATA_CLIENT_VERSION


def print_results(responses, stream):
    if responses is None:
        print("Empty results - None object")
        return

    for response in responses:
        if response is None:
            print("Empty results - skipping response")
            continue

        print("Received response with status: {}".format(ResponseStatus.Name(response.status)))

        if response.error:
            print("[ERROR]: {}".format(response.error))

        # single response expected
        processing_completed = True
        if response.status == START_OF_INPUT or response.status == EMPTY:
            processing_completed = False

        if processing_completed:
            stream.close()

        n = 1
        for res in response.results:
            transcript = " ".join([word.transcript for word in res.words])
            print("[{}.] {} /{}/ ({})".format(n, transcript, res.semantic_interpretation, res.confidence))
            n += 1


def validate_recognition_settings(settings):
    if not settings.grammar_name and not settings.grammar:
        print("Bad usage. Recognize usage: `sarmata_client.py --address <service_addres> "
              "[--wave <wave_file> | --mic] {--grammar-name <grammar_name>, --grammar <grammar_file>}`")
        sys.exit(1)


def create_audio_stream(args):
    # create wave file stream
    if args.wave is not None:
        return AudioStream(args.wave)

    # create microphone stream
    if args.mic:
        rate = 16000  # [Hz]
        chunk = int(rate / 10)  # [100 ms]
        return MicrophoneStream(rate, chunk)

    # default
    raise ValueError("Unknown media source to create")


if __name__ == '__main__':
    print("Sarmata client " + SARMATA_CLIENT_VERSION)

    parser = ArgumentParser(description="""Main script for running tests of Techmo Sarmata ASR system""")
    parser.add_argument("--address", help="Techmo Sarmata ASR service", required=True)
    parser.add_argument("--define-grammar", help="Defines a new grammar to be cached by the service under ID "
                                                 "given by `--grammar-id` option from data given by `--grammar` option",
                        action='store_true')
    parser.add_argument("--grammar-name", help="Name (ID) of the grammar in the service's grammar cache", default='',
                        type=str)
    parser.add_argument("--grammar", help="SRGS grammar file (ABNF or XML format accepted)")
    parser.add_argument("--wave", help="Wave path, should be mono, 8kHz or 16kHz")
    parser.add_argument("--mic", help="Use microphone as an audio source (instead of wave file)", action='store_true')

    # Timeouts, settings
    parser.add_argument("--nbest", help="Maximal number of hypotheses", default=3, type=int)
    parser.add_argument("--nomatch", help="Confidence acceptance threshold", default=0.2, type=float)
    parser.add_argument("--speech-complete", help="MRCP v2 speech complete timeout [ms]", default=500, type=int)
    parser.add_argument("--speech-incomplete", help="MRCP v2 speech incomplete timeout [ms]", default=3000, type=int)
    parser.add_argument("--no-input", help="MRCP v2 no input timeout [ms]", default=5000, type=int)
    parser.add_argument("--recognition-timeout", help="MRCP v2 recognition timeout [ms]", default=10000, type=int)

    args = parser.parse_args()

    settings = SarmataSettings()
    settings.process_args(args) # load settings from cmd
    if args.grammar is not None:
        settings.load_grammar(args.grammar)

    can_define_grammar = False
    if args.define_grammar:
        if not settings.grammar_name:
            print("Bad usage. Define grammar usage: `sarmata_client.py --address <service_address> "
                  "--define-grammar --grammar-name <grammar_name> --grammar <grammar_file>`")
            sys.exit(1)
        can_define_grammar = True

    recognizer = SarmataRecognizer(args.address)

    if can_define_grammar:
        define_grammar_response = recognizer.define_grammar(args.grammar_name, settings.grammar)
        if define_grammar_response.ok:
            if args.grammar == None:
                print("Grammar " + args.grammar_name + " removed")
            else:
                print("Grammar " + args.grammar + " defined as " + args.grammar_name)
        else:
            print("Define grammar error: " + define_grammar_response.error)

    # --------------------------
    # recognize section
    # --------------------------
    if args.wave is not None or args.mic:
        validate_recognition_settings(settings)

        with create_audio_stream(args) as stream:
            # generate id
            session_id = stream.session_id()
            settings.set_session_id(session_id)

            results = recognizer.recognize(stream, settings)
            print_results(results, stream)
