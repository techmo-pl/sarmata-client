// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: sarmata_asr.proto
// Original file comments:
// Techmo Sarmata ASR API
// version: 2.0.0
// authors: Dawid Skurzok, Paweł Jaciów
// date:    2018-02-26
//
// Some content is derived from:
// https://github.com/googleapis/googleapis/blob/master/google/cloud/speech/v1/cloud_speech.proto
#ifndef GRPC_sarmata_5fasr_2eproto__INCLUDED
#define GRPC_sarmata_5fasr_2eproto__INCLUDED

#include "sarmata_asr.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace grpc_impl {
class CompletionQueue;
class ServerCompletionQueue;
class ServerContext;
}  // namespace grpc_impl

namespace grpc {
namespace experimental {
template <typename RequestT, typename ResponseT>
class MessageAllocator;
}  // namespace experimental
}  // namespace grpc

namespace techmo {
namespace sarmata {

// Service that implements Techmo Automatic-Speech-Recognition (ASR) API.
//
// Grammar persistence options:
// - per alive connection: user opens special channel to service to define grammars, grammars are kept as long as connection is alive,
// - predefined per user on disk: service keep predefined grammar as files on disk, loads it at startup,
// - auto-cache: all grammars are cached automatically using its hash as ID, max number of cached grammars and prune policy must be defined.
//
// Supported grammar formats are XML and ABNF as specified by [W3C SRGS](https://www.w3.org/TR/speech-grammar/).
class ASR final {
 public:
  static constexpr char const* service_full_name() {
    return "techmo.sarmata.ASR";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    // Performs bidirectional streaming speech recognition using given grammar.
    // Receive results while sending audio.
    std::unique_ptr< ::grpc::ClientReaderWriterInterface< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>> Recognize(::grpc::ClientContext* context) {
      return std::unique_ptr< ::grpc::ClientReaderWriterInterface< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>>(RecognizeRaw(context));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>> AsyncRecognize(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>>(AsyncRecognizeRaw(context, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>> PrepareAsyncRecognize(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>>(PrepareAsyncRecognizeRaw(context, cq));
    }
    // Defines grammar that will be stored for future use in recognitions
    // requested by `Recognize`.
    virtual ::grpc::Status DefineGrammar(::grpc::ClientContext* context, const ::techmo::sarmata::DefineGrammarRequest& request, ::techmo::sarmata::DefineGrammarResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::techmo::sarmata::DefineGrammarResponse>> AsyncDefineGrammar(::grpc::ClientContext* context, const ::techmo::sarmata::DefineGrammarRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::techmo::sarmata::DefineGrammarResponse>>(AsyncDefineGrammarRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::techmo::sarmata::DefineGrammarResponse>> PrepareAsyncDefineGrammar(::grpc::ClientContext* context, const ::techmo::sarmata::DefineGrammarRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::techmo::sarmata::DefineGrammarResponse>>(PrepareAsyncDefineGrammarRaw(context, request, cq));
    }
    class experimental_async_interface {
     public:
      virtual ~experimental_async_interface() {}
      // Performs bidirectional streaming speech recognition using given grammar.
      // Receive results while sending audio.
      virtual void Recognize(::grpc::ClientContext* context, ::grpc::experimental::ClientBidiReactor< ::techmo::sarmata::RecognizeRequest,::techmo::sarmata::RecognizeResponse>* reactor) = 0;
      // Defines grammar that will be stored for future use in recognitions
      // requested by `Recognize`.
      virtual void DefineGrammar(::grpc::ClientContext* context, const ::techmo::sarmata::DefineGrammarRequest* request, ::techmo::sarmata::DefineGrammarResponse* response, std::function<void(::grpc::Status)>) = 0;
      virtual void DefineGrammar(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::techmo::sarmata::DefineGrammarResponse* response, std::function<void(::grpc::Status)>) = 0;
      virtual void DefineGrammar(::grpc::ClientContext* context, const ::techmo::sarmata::DefineGrammarRequest* request, ::techmo::sarmata::DefineGrammarResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      virtual void DefineGrammar(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::techmo::sarmata::DefineGrammarResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
    };
    virtual class experimental_async_interface* experimental_async() { return nullptr; }
  private:
    virtual ::grpc::ClientReaderWriterInterface< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>* RecognizeRaw(::grpc::ClientContext* context) = 0;
    virtual ::grpc::ClientAsyncReaderWriterInterface< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>* AsyncRecognizeRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientAsyncReaderWriterInterface< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>* PrepareAsyncRecognizeRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::techmo::sarmata::DefineGrammarResponse>* AsyncDefineGrammarRaw(::grpc::ClientContext* context, const ::techmo::sarmata::DefineGrammarRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::techmo::sarmata::DefineGrammarResponse>* PrepareAsyncDefineGrammarRaw(::grpc::ClientContext* context, const ::techmo::sarmata::DefineGrammarRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    std::unique_ptr< ::grpc::ClientReaderWriter< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>> Recognize(::grpc::ClientContext* context) {
      return std::unique_ptr< ::grpc::ClientReaderWriter< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>>(RecognizeRaw(context));
    }
    std::unique_ptr<  ::grpc::ClientAsyncReaderWriter< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>> AsyncRecognize(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriter< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>>(AsyncRecognizeRaw(context, cq, tag));
    }
    std::unique_ptr<  ::grpc::ClientAsyncReaderWriter< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>> PrepareAsyncRecognize(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriter< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>>(PrepareAsyncRecognizeRaw(context, cq));
    }
    ::grpc::Status DefineGrammar(::grpc::ClientContext* context, const ::techmo::sarmata::DefineGrammarRequest& request, ::techmo::sarmata::DefineGrammarResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::techmo::sarmata::DefineGrammarResponse>> AsyncDefineGrammar(::grpc::ClientContext* context, const ::techmo::sarmata::DefineGrammarRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::techmo::sarmata::DefineGrammarResponse>>(AsyncDefineGrammarRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::techmo::sarmata::DefineGrammarResponse>> PrepareAsyncDefineGrammar(::grpc::ClientContext* context, const ::techmo::sarmata::DefineGrammarRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::techmo::sarmata::DefineGrammarResponse>>(PrepareAsyncDefineGrammarRaw(context, request, cq));
    }
    class experimental_async final :
      public StubInterface::experimental_async_interface {
     public:
      void Recognize(::grpc::ClientContext* context, ::grpc::experimental::ClientBidiReactor< ::techmo::sarmata::RecognizeRequest,::techmo::sarmata::RecognizeResponse>* reactor) override;
      void DefineGrammar(::grpc::ClientContext* context, const ::techmo::sarmata::DefineGrammarRequest* request, ::techmo::sarmata::DefineGrammarResponse* response, std::function<void(::grpc::Status)>) override;
      void DefineGrammar(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::techmo::sarmata::DefineGrammarResponse* response, std::function<void(::grpc::Status)>) override;
      void DefineGrammar(::grpc::ClientContext* context, const ::techmo::sarmata::DefineGrammarRequest* request, ::techmo::sarmata::DefineGrammarResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      void DefineGrammar(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::techmo::sarmata::DefineGrammarResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
     private:
      friend class Stub;
      explicit experimental_async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class experimental_async_interface* experimental_async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class experimental_async async_stub_{this};
    ::grpc::ClientReaderWriter< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>* RecognizeRaw(::grpc::ClientContext* context) override;
    ::grpc::ClientAsyncReaderWriter< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>* AsyncRecognizeRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) override;
    ::grpc::ClientAsyncReaderWriter< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>* PrepareAsyncRecognizeRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::techmo::sarmata::DefineGrammarResponse>* AsyncDefineGrammarRaw(::grpc::ClientContext* context, const ::techmo::sarmata::DefineGrammarRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::techmo::sarmata::DefineGrammarResponse>* PrepareAsyncDefineGrammarRaw(::grpc::ClientContext* context, const ::techmo::sarmata::DefineGrammarRequest& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_Recognize_;
    const ::grpc::internal::RpcMethod rpcmethod_DefineGrammar_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    // Performs bidirectional streaming speech recognition using given grammar.
    // Receive results while sending audio.
    virtual ::grpc::Status Recognize(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::techmo::sarmata::RecognizeResponse, ::techmo::sarmata::RecognizeRequest>* stream);
    // Defines grammar that will be stored for future use in recognitions
    // requested by `Recognize`.
    virtual ::grpc::Status DefineGrammar(::grpc::ServerContext* context, const ::techmo::sarmata::DefineGrammarRequest* request, ::techmo::sarmata::DefineGrammarResponse* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_Recognize : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_Recognize() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_Recognize() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Recognize(::grpc::ServerContext* /*context*/, ::grpc::ServerReaderWriter< ::techmo::sarmata::RecognizeResponse, ::techmo::sarmata::RecognizeRequest>* /*stream*/)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestRecognize(::grpc::ServerContext* context, ::grpc::ServerAsyncReaderWriter< ::techmo::sarmata::RecognizeResponse, ::techmo::sarmata::RecognizeRequest>* stream, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncBidiStreaming(0, context, stream, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_DefineGrammar : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_DefineGrammar() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_DefineGrammar() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DefineGrammar(::grpc::ServerContext* /*context*/, const ::techmo::sarmata::DefineGrammarRequest* /*request*/, ::techmo::sarmata::DefineGrammarResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestDefineGrammar(::grpc::ServerContext* context, ::techmo::sarmata::DefineGrammarRequest* request, ::grpc::ServerAsyncResponseWriter< ::techmo::sarmata::DefineGrammarResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_Recognize<WithAsyncMethod_DefineGrammar<Service > > AsyncService;
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_Recognize : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithCallbackMethod_Recognize() {
      ::grpc::Service::experimental().MarkMethodCallback(0,
        new ::grpc_impl::internal::CallbackBidiHandler< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>(
          [this] { return this->Recognize(); }));
    }
    ~ExperimentalWithCallbackMethod_Recognize() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Recognize(::grpc::ServerContext* /*context*/, ::grpc::ServerReaderWriter< ::techmo::sarmata::RecognizeResponse, ::techmo::sarmata::RecognizeRequest>* /*stream*/)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::experimental::ServerBidiReactor< ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>* Recognize() {
      return new ::grpc_impl::internal::UnimplementedBidiReactor<
        ::techmo::sarmata::RecognizeRequest, ::techmo::sarmata::RecognizeResponse>;}
  };
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_DefineGrammar : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithCallbackMethod_DefineGrammar() {
      ::grpc::Service::experimental().MarkMethodCallback(1,
        new ::grpc_impl::internal::CallbackUnaryHandler< ::techmo::sarmata::DefineGrammarRequest, ::techmo::sarmata::DefineGrammarResponse>(
          [this](::grpc::ServerContext* context,
                 const ::techmo::sarmata::DefineGrammarRequest* request,
                 ::techmo::sarmata::DefineGrammarResponse* response,
                 ::grpc::experimental::ServerCallbackRpcController* controller) {
                   return this->DefineGrammar(context, request, response, controller);
                 }));
    }
    void SetMessageAllocatorFor_DefineGrammar(
        ::grpc::experimental::MessageAllocator< ::techmo::sarmata::DefineGrammarRequest, ::techmo::sarmata::DefineGrammarResponse>* allocator) {
      static_cast<::grpc_impl::internal::CallbackUnaryHandler< ::techmo::sarmata::DefineGrammarRequest, ::techmo::sarmata::DefineGrammarResponse>*>(
          ::grpc::Service::experimental().GetHandler(1))
              ->SetMessageAllocator(allocator);
    }
    ~ExperimentalWithCallbackMethod_DefineGrammar() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DefineGrammar(::grpc::ServerContext* /*context*/, const ::techmo::sarmata::DefineGrammarRequest* /*request*/, ::techmo::sarmata::DefineGrammarResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual void DefineGrammar(::grpc::ServerContext* /*context*/, const ::techmo::sarmata::DefineGrammarRequest* /*request*/, ::techmo::sarmata::DefineGrammarResponse* /*response*/, ::grpc::experimental::ServerCallbackRpcController* controller) { controller->Finish(::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "")); }
  };
  typedef ExperimentalWithCallbackMethod_Recognize<ExperimentalWithCallbackMethod_DefineGrammar<Service > > ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_Recognize : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_Recognize() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_Recognize() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Recognize(::grpc::ServerContext* /*context*/, ::grpc::ServerReaderWriter< ::techmo::sarmata::RecognizeResponse, ::techmo::sarmata::RecognizeRequest>* /*stream*/)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_DefineGrammar : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_DefineGrammar() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_DefineGrammar() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DefineGrammar(::grpc::ServerContext* /*context*/, const ::techmo::sarmata::DefineGrammarRequest* /*request*/, ::techmo::sarmata::DefineGrammarResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_Recognize : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_Recognize() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_Recognize() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Recognize(::grpc::ServerContext* /*context*/, ::grpc::ServerReaderWriter< ::techmo::sarmata::RecognizeResponse, ::techmo::sarmata::RecognizeRequest>* /*stream*/)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestRecognize(::grpc::ServerContext* context, ::grpc::ServerAsyncReaderWriter< ::grpc::ByteBuffer, ::grpc::ByteBuffer>* stream, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncBidiStreaming(0, context, stream, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_DefineGrammar : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_DefineGrammar() {
      ::grpc::Service::MarkMethodRaw(1);
    }
    ~WithRawMethod_DefineGrammar() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DefineGrammar(::grpc::ServerContext* /*context*/, const ::techmo::sarmata::DefineGrammarRequest* /*request*/, ::techmo::sarmata::DefineGrammarResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestDefineGrammar(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_Recognize : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithRawCallbackMethod_Recognize() {
      ::grpc::Service::experimental().MarkMethodRawCallback(0,
        new ::grpc_impl::internal::CallbackBidiHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
          [this] { return this->Recognize(); }));
    }
    ~ExperimentalWithRawCallbackMethod_Recognize() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Recognize(::grpc::ServerContext* /*context*/, ::grpc::ServerReaderWriter< ::techmo::sarmata::RecognizeResponse, ::techmo::sarmata::RecognizeRequest>* /*stream*/)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::experimental::ServerBidiReactor< ::grpc::ByteBuffer, ::grpc::ByteBuffer>* Recognize() {
      return new ::grpc_impl::internal::UnimplementedBidiReactor<
        ::grpc::ByteBuffer, ::grpc::ByteBuffer>;}
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_DefineGrammar : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithRawCallbackMethod_DefineGrammar() {
      ::grpc::Service::experimental().MarkMethodRawCallback(1,
        new ::grpc_impl::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
          [this](::grpc::ServerContext* context,
                 const ::grpc::ByteBuffer* request,
                 ::grpc::ByteBuffer* response,
                 ::grpc::experimental::ServerCallbackRpcController* controller) {
                   this->DefineGrammar(context, request, response, controller);
                 }));
    }
    ~ExperimentalWithRawCallbackMethod_DefineGrammar() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DefineGrammar(::grpc::ServerContext* /*context*/, const ::techmo::sarmata::DefineGrammarRequest* /*request*/, ::techmo::sarmata::DefineGrammarResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual void DefineGrammar(::grpc::ServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/, ::grpc::experimental::ServerCallbackRpcController* controller) { controller->Finish(::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "")); }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_DefineGrammar : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_DefineGrammar() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::internal::StreamedUnaryHandler< ::techmo::sarmata::DefineGrammarRequest, ::techmo::sarmata::DefineGrammarResponse>(std::bind(&WithStreamedUnaryMethod_DefineGrammar<BaseClass>::StreamedDefineGrammar, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_DefineGrammar() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status DefineGrammar(::grpc::ServerContext* /*context*/, const ::techmo::sarmata::DefineGrammarRequest* /*request*/, ::techmo::sarmata::DefineGrammarResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedDefineGrammar(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::techmo::sarmata::DefineGrammarRequest,::techmo::sarmata::DefineGrammarResponse>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_DefineGrammar<Service > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_DefineGrammar<Service > StreamedService;
};

}  // namespace sarmata
}  // namespace techmo


#endif  // GRPC_sarmata_5fasr_2eproto__INCLUDED
