#ifndef COMMUNICATION_STREAM_TCP_SERVER_H_
#define COMMUNICATION_STREAM_TCP_SERVER_H_

#include <string>
#include <queue>

#include <SFML/Network.hpp>

#include "c24/communication/stream_tcp.h"

namespace c24 {
namespace communication {

// Implementation of StreamBackendInterface for tcp server.
class StreamTcpServer : public StreamTcp {
 public:
  // Listen for connection at specified port. Blocks until connection is
  // established.
  StreamTcpServer(unsigned short port);
};

}  // namespace communication
}  // namespace c24

#endif //  COMMUNICATION_STREAM_TCP_SERVER_H_
