from . import sarmata_asr_pb2
from . import sarmata_asr_pb2_grpc
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
            True: self._config_request,
            False: self._normal_request
        }
        # Iterator state
        self.lock = threading.Lock()
        self.is_config_request = True
        self.eos = False  # indicates whether end of stream message was send (request to stop iterator)

    def _config_request(self):
        request = sarmata_asr_pb2.RecognizeRequest(
            config=sarmata_asr_pb2.RecognitionConfig(
                sample_rate_hertz=self.audio_stream.frame_rate(),
                max_alternatives=self.settings.nbest
            )
        )

        if self.settings.grammar_name:
            request.config.name = self.settings.grammar_name
        elif self.settings.grammar:
            request.config.data = self.settings.grammar
        else:
            raise ValueError("Grammar must be loaded or grammar name must be set first")

        settings_map = self.settings.to_map()
        for key in settings_map:
            cf = request.config.config.add()
            cf.key = key
            cf.value = str(settings_map[key])

        self.is_config_request = False
        return request

    def _normal_request(self):
        data = next(self.audio_generator)
        return sarmata_asr_pb2.RecognizeRequest(audio_content=data)

    def __iter__(self):
        return self

    def __next__(self):
        with self.lock:
            return self.request_builder[self.is_config_request]()


class SarmataRecognizer:

    def __init__(self, address):
        self.service = SarmataRecognizer.connect(address)

    def recognize(self, audio_stream, settings):
        metadata = []
        if settings.session_id:
            metadata = [('session_id', settings.session_id)]
        requests_iterator = RequestIterator(audio_stream, settings)
        return self.service.Recognize(requests_iterator, metadata=metadata)

    def define_grammar(self, grammar_name, grammar):
        request = sarmata_asr_pb2.DefineGrammarRequest(name=grammar_name, grammar=grammar)
        response = self.service.DefineGrammar(request)
        return response

    @staticmethod
    def connect(endpoint):
        service = sarmata_asr_pb2_grpc.ASRStub(
            grpc.insecure_channel(endpoint))
        return service
