#ifndef COMMUNICATION_STREAM_TCP_H_
#define COMMUNICATION_STREAM_TCP_H_

#include <string>
#include <queue>

#include <SFML/Network.hpp>

#include "c24/communication/stream_backend_interface.h"

// Specifies the maximum number of characters received at once.
#ifndef _TCP_BUFFER_SIZE
#define _TCP_BUFFER_SIZE 100
#endif  // _TCP_BUFFER_SIZE

namespace c24 {
namespace communication {

// Implementation of StreamBackendInterface for tcp streams -- both client and
// server. Constructor is protected so that the class is abstract.
class StreamTcp : public StreamBackendInterface {
 public:
  virtual ~StreamTcp() {}

  // Non-blocking call to check if there is a message available on the stream.
  bool MessageAvailable() override;

  // Blocking call that receives one line. Returned string does not contain the
  // newline character.
  std::string GetMessage() override;

  // Blocking call that sends a message (without newline). Returns true on
  // success.
  bool SendMessage(const std::string& msg) override;

  // Returns if the connection is still valid.
  bool Connected() const override { return connected_; }
 protected:
  StreamTcp() {}

  sf::TcpSocket socket_;
  bool connected_;
 private:
  // Get raw data from the socket and if successful, process it.
  sf::TcpSocket::Status GetData(std::size_t* received);

  // For storing received raw data.
  char buffer_[_TCP_BUFFER_SIZE + 1];
  // Part of the last message -- not yet terminated by newline character.
  std::string not_complete_message_;
  // Queue of all received messages not yet obtained by GetMessage method.
  std::queue<std::string> complete_messages_;
};

}  // namespace communication
}  // namespace c24

#endif //  COMMUNICATION_STREAM_TCP_H_
