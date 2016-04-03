#include "server_stream.h"

namespace c24 {
namespace communication {

ServerStream::ServerStream(std::function<
    std::unique_ptr<StreamBackendInterface>()> get_stream_backend) {
  Connect(get_stream_backend);
}

void ServerStream::ConnectAgain() { Connect(last_get_stream_backend_); }

void ServerStream::Connect(std::function<
    std::unique_ptr<StreamBackendInterface>()> get_stream_backend) {
  waiting_for_future_stream_ = true;
  last_get_stream_backend_ = get_stream_backend;
  // Construct the stream in different thread.
  future_stream_ = std::async(std::launch::async, [=] {
    return Stream(std::move(get_stream_backend()));
  });
}

bool ServerStream::Connected() {
  // If there is new connection ready, connect.
  if (waiting_for_future_stream_ &&
      future_stream_.wait_for(std::chrono::seconds(0)) ==
          std::future_status::ready) {
    SetStreamFromFuture();
  }
  return stream_ != nullptr && stream_->Connected();
}

bool ServerStream::Disconnected() {
  return !waiting_for_future_stream_ && !Connected();
}

bool ServerStream::MessageAvailable() {
  return Connected() && stream_->MessageAvailable();
}

std::string ServerStream::GetMessage() {
  if (Connected()) {
    return stream_->GetMessage();
  }
  return "";
}
bool ServerStream::SendMessage(const std::string& msg) {
  if (Connected()) {
    return stream_->SendMessage(msg);
  }
  return false;
}

Stream* ServerStream::GetStream() {
  if (Connected()) {
    return stream_.get();
  }
  return nullptr;
}

void ServerStream::SetStreamFromFuture() {
  stream_ = std::unique_ptr<Stream>(new Stream(future_stream_.get()));
  waiting_for_future_stream_ = false;
}

}  // namespace communication
}  // namespace c24
