#include "c24/communication/stream_tcp_client.h"

#include <glog/logging.h>

namespace c24 {
namespace communication {

StreamTcpClient::StreamTcpClient(const sf::IpAddress& ip_address,
                                 unsigned short port) {
  sf::Socket::Status status = socket_.connect(ip_address, port);
  if (status == sf::Socket::Done) {
    connected_ = true;
  } else {
    connected_ = false;
    LOG(ERROR) << "Unable to connect to host. IP address: " << ip_address
               << ", port: " << port;
  }
}

}  // namespace communication
}  // namespace c24
