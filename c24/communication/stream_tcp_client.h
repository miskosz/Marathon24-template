#ifndef COMMUNICATION_STREAM_TCP_CLIENT_H_
#define COMMUNICATION_STREAM_TCP_CLIENT_H_

#include <string>
#include <queue>

#include <SFML/Network.hpp>

#include "c24/communication/stream_tcp.h"

namespace c24 {
namespace communication {

// Implementation of StreamBackendInterface for tcp client.
class StreamTcpClient : public StreamTcp {
 public:
  // Connect to server at specified location.
  StreamTcpClient(const sf::IpAddress& ip_address, unsigned short port);
  StreamTcpClient(const std::string& ip_address, unsigned short port)
      : StreamTcpClient(sf::IpAddress(ip_address), port) {}
  StreamTcpClient(const char* ip_address, unsigned short port)
      : StreamTcpClient(sf::IpAddress(ip_address), port) {}
};

}  // namespace communication
}  // namespace c24

#endif //  COMMUNICATION_STREAM_TCP_CLIENT_H_
