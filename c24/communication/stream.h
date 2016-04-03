#ifndef COMMUNICATION_STREAM_H_
#define COMMUNICATION_STREAM_H_

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "c24/communication/stream_backend_interface.h"

namespace c24 {
namespace communication {

// Class for data exchange. It wraps some implementation of
// StreamBackendInterface, logs the communication and provides some useful
// methods for convenience.
class Stream {
 public:
  Stream() {}

  // TODO(simsa.st): Add an argument to the constructor that specifies where to
  // log the communication.
  Stream(std::unique_ptr<StreamBackendInterface> stream_backend);

  // Return if the other side of the stream is still connected.
  bool Connected() const;

  // Non-blocking call that returns if there is a message available on the
  // stream.
  bool MessageAvailable() const;

  // Blocking call that receives a message without the newline character.
  std::string GetMessage() const;

  // Send a message and if 'newline' is true, append newline. Returns true if
  // the message was sent successfully.
  bool SendMessage(const std::string& msg, bool newline = true) const;

  // Send a message, then receive reply and check if it is equal to 'expected'.
  // Returns true if the message was sent succesfully and expected value
  // (without newline) was received.
  bool SendMessageWithCheck(const std::string& msg, bool newline = true,
                            const char* expected = "OK") const;

  // Receive a message and extract "cnt" variables of type T from it using
  // stringstream. Also return rest of the message in the second argument if it
  // is not nullptr.
  template <typename T>
  std::vector<T> GetVectorOf(int cnt, std::string* rest_of_msg);

  // When you don't need the rest of the message.
  template <typename T>
  std::vector<T> GetVectorOf(int cnt) {
    return GetVectorOf<T>(cnt, nullptr);
  }

 private:
  std::unique_ptr<StreamBackendInterface> stream_backend_;
};

// Implementation needs to be in the header file because of the template.
template <typename T>
std::vector<T> Stream::GetVectorOf(int cnt, std::string* rest_of_msg) {
  std::string msg = GetMessage();
  std::stringstream sstream(msg);
  std::vector<T> vector_of;
  for (int i = 0; i < cnt; ++i) {
    T val;
    sstream >> val;
    vector_of.push_back(val);
  }
  if (rest_of_msg != nullptr) {
    *rest_of_msg = sstream.str();
  }
  return vector_of;
}

}  // namespace communication
}  // namespace c24
#endif //  COMMUNICATION_STREAM_H_
