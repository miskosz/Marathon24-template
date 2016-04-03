#include "c24/communication/stream.h"

#include <string>
#include <glog/logging.h>

namespace c24 {
namespace communication {

Stream::Stream(std::unique_ptr<StreamBackendInterface> stream_backend)
    : stream_backend_(std::move(stream_backend)) {}

bool Stream::Connected() const {
  if (stream_backend_ == nullptr) {
    return false;
  }
  return stream_backend_->Connected();
}

bool Stream::MessageAvailable() const {
  CHECK(stream_backend_ != nullptr);
  return stream_backend_->MessageAvailable();
}

std::string Stream::GetMessage() const {
  CHECK(stream_backend_ != nullptr);
  std::string msg = stream_backend_->GetMessage();
  LOG(INFO) << "Received: \"" << msg << "\"";
  return msg;
}

bool Stream::SendMessage(const std::string& msg, bool newline) const {
  CHECK(stream_backend_ != nullptr);
  // Put two spaces after SENDING so it is lined up with messages after
  // RECEIVED.
  LOG(INFO) << "Sending:  \"" << msg << "\"";
  const std::string& msg_to_send = newline ? msg + '\n' : msg;
  return stream_backend_->SendMessage(msg_to_send);
}

bool Stream::SendMessageWithCheck(const std::string& msg, bool newline,
                                  const char* expected) const {
  bool success = SendMessage(msg, newline);
  std::string msg_ok = GetMessage();
  if (msg_ok != expected) {
    LOG(ERROR) << "Expected \"" << expected << "\" but received \"" << msg_ok
               << "\"";
    success = false;
  }
  return success;
}

}  // namespace communication
}  // namespace c24
