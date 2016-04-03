#ifndef COMMUNICATION_STREAM_BACKEND_INTERFACE_H_
#define COMMUNICATION_STREAM_BACKEND_INTERFACE_H_

#include <string>

namespace c24 {
namespace communication {

// Interface for basic streams -- sending and receiving messages. Class that
// implements this interface is just backend for sending and receiving raw
// messages. It is then wrapped in Stream class that logs the communication.
// Avoid using StreamBackendInterface directly.
class StreamBackendInterface {
 public:
  virtual ~StreamBackendInterface() {}

  // Non-blocking call to check if there is a message available on the stream.
  virtual bool MessageAvailable() = 0;

  // Blocking call that receives one line. Returned string does not contain the
  // newline character.
  virtual std::string GetMessage() = 0;

  // Blocking call that sends a message (without newline). Returns true on
  // success.
  virtual bool SendMessage(const std::string& msg) = 0;

  // Returns if the connection is still valid.
  virtual bool Connected() const = 0;
};

}  // namespace communication
}  // namespace c24

#endif //  COMMUNICATION_STREAM_BACKEND_INTERFACE_H_
