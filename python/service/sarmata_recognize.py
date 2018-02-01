from . import asr_service_pb2
from . import asr_service_pb2_grpc
import grpc
import threading


class RequestIterator:
    """Thread-safe request iterator for streaming recognizer."""

    def __init__(self, audio_stream, settings):
        # Iterator data
        self.audio_stream = audio_stream
        self.audio_generator = self.audio_stream.generator()

        self.settings = settings

        self.request_builder = {
            True: self._initial_request,
            False: self._normal_request
        }
        # Iterator state
        self.lock = threading.Lock()
        self.is_initial_request = True
        self.eos = False  # indicates whether end of stream message was send (request to stop iterator)

    def _initial_request(self):
        request = asr_service_pb2.RecognizeRequest(
            initial_request=asr_service_pb2.InitialRecognizeRequest(
            )
        )

        settings_map = self.settings.to_map()
        for key in settings_map:
            cf = request.initial_request.config.add()
            cf.key = key
            cf.value = str(settings_map[key])

        # add sampling rate
        cf = request.initial_request.config.add()
        cf.key = "sampling-rate"
        cf.value = str(self.audio_stream.frame_rate())

        self.is_initial_request = False
        return request

    def _normal_request(self):
        # EOS already sent, stop iteration
        if self.eos:
            raise StopIteration()

        try:
            data = next(self.audio_generator)
            return asr_service_pb2.RecognizeRequest(audio_request=asr_service_pb2.AudioRequest(
                content=data,
                end_of_stream=False
                )
            )
        except StopIteration:
            # send only EndOfStream indicator
            self.eos = True
            return asr_service_pb2.RecognizeRequest(audio_request=asr_service_pb2.AudioRequest(
                end_of_stream=True
            ))

    def __iter__(self):
        return self

    def __next__(self):
        with self.lock:
            return self.request_builder[self.is_initial_request]()


class SarmataRecognizer:

    def __init__(self, address):
        self.service = SarmataRecognizer.connect(address)

    def recognize(self, audio_stream, settings):
        requests_iterator = RequestIterator(audio_stream, settings)
        return self.service.Recognize(requests_iterator)

    def define_grammar(self, grammar_name, grammar):
        request = asr_service_pb2.DefineGrammarRequest(name=grammar_name, grammar=grammar)
        response = self.service.DefineGrammar(request)
        return response

    @staticmethod
    def connect(endpoint):
        service = asr_service_pb2_grpc.ASRStub(
            grpc.insecure_channel(endpoint))
        return service
