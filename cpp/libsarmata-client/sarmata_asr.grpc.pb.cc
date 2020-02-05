// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: sarmata_asr.proto

#include "sarmata_asr.pb.h"
#include "sarmata_asr.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace techmo {
namespace sarmata {

static const char* ASR_method_names[] = {
  "/techmo.sarmata.ASR/Recognize",
  "/techmo.sarmata.ASR/DefineGrammar",
};

std::unique_ptr< ASR::Stub> ASR::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< ASR::Stub> stub(new ASR::Stub(channel));
  return stub;
}

ASR::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_Recognize_(ASR_method_names[0], ::grpc::internal::RpcMethod::BIDI_STREAMING, channel)
  , rpcmethod_DefineGrammar_(ASR_method_names[1], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::ClientReaderWriter< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>* ASR::Stub::RecognizeRaw(::grpc::ClientContext* context) {
  return ::grpc_impl::internal::ClientReaderWriterFactory< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>::Create(channel_.get(), rpcmethod_Recognize_, context);
}

void ASR::Stub::experimental_async::Recognize(::grpc::ClientContext* context, ::grpc::experimental::ClientBidiReactor< ::techmo::sarmata::RecognizeRequest,::techmo::sarmata::RecognizeResponse>* reactor) {
  ::grpc_impl::internal::ClientCallbackReaderWriterFactory< ::techmo::sarmata::RecognizeRequest,::techmo::sarmata::RecognizeResponse>::Create(stub_->channel_.get(), stub_->rpcmethod_Recognize_, context, reactor);
}

::grpc::ClientAsyncReaderWriter< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>* ASR::Stub::AsyncRecognizeRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>::Create(channel_.get(), cq, rpcmethod_Recognize_, context, true, tag);
}

::grpc::ClientAsyncReaderWriter< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>* ASR::Stub::PrepareAsyncRecognizeRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>::Create(channel_.get(), cq, rpcmethod_Recognize_, context, false, nullptr);
}

::grpc::Status ASR::Stub::DefineGrammar(::grpc::ClientContext* context, const ::techmo::sarmata::DefineGrammarRequest& request, ::techmo::sarmata::DefineGrammarResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_DefineGrammar_, context, request, response);
}

void ASR::Stub::experimental_async::DefineGrammar(::grpc::ClientContext* context, const ::techmo::sarmata::DefineGrammarRequest* request, ::techmo::sarmata::DefineGrammarResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_DefineGrammar_, context, request, response, std::move(f));
}

void ASR::Stub::experimental_async::DefineGrammar(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::techmo::sarmata::DefineGrammarResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_DefineGrammar_, context, request, response, std::move(f));
}

void ASR::Stub::experimental_async::DefineGrammar(::grpc::ClientContext* context, const ::techmo::sarmata::DefineGrammarRequest* request, ::techmo::sarmata::DefineGrammarResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_DefineGrammar_, context, request, response, reactor);
}

void ASR::Stub::experimental_async::DefineGrammar(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::techmo::sarmata::DefineGrammarResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_DefineGrammar_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::techmo::sarmata::DefineGrammarResponse>* ASR::Stub::AsyncDefineGrammarRaw(::grpc::ClientContext* context, const ::techmo::sarmata::DefineGrammarRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::techmo::sarmata::DefineGrammarResponse>::Create(channel_.get(), cq, rpcmethod_DefineGrammar_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::techmo::sarmata::DefineGrammarResponse>* ASR::Stub::PrepareAsyncDefineGrammarRaw(::grpc::ClientContext* context, const ::techmo::sarmata::DefineGrammarRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::techmo::sarmata::DefineGrammarResponse>::Create(channel_.get(), cq, rpcmethod_DefineGrammar_, context, request, false);
}

ASR::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ASR_method_names[0],
      ::grpc::internal::RpcMethod::BIDI_STREAMING,
      new ::grpc::internal::BidiStreamingHandler< ASR::Service, ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>(
          std::mem_fn(&ASR::Service::Recognize), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ASR_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< ASR::Service, ::techmo::sarmata::DefineGrammarRequest, ::techmo::sarmata::DefineGrammarResponse>(
          std::mem_fn(&ASR::Service::DefineGrammar), this)));
}

ASR::Service::~Service() {
}

::grpc::Status ASR::Service::Recognize(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::techmo::sarmata::RecognizeResponse, ::techmo::sarmata::RecognizeRequest>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status ASR::Service::DefineGrammar(::grpc::ServerContext* context, const ::techmo::sarmata::DefineGrammarRequest* request, ::techmo::sarmata::DefineGrammarResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace techmo
}  // namespace sarmata

