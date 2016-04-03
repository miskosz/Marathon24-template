#include "c24/communication/stream_tcp_server.h"

#include <glog/logging.h>

namespace c24 {
namespace communication {

StreamTcpServer::StreamTcpServer(unsigned short port) {
  sf::TcpListener listener;
  // bind the listener to a port
  sf::Socket::Status bind_status = listener.listen(port);
  if (bind_status != sf::Socket::Done) {
    connected_ = false;
    LOG(ERROR) << "Could not bind to port " << port
               << ", status: " << bind_status;
  } else {
    sf::Socket::Status accept_status = listener.accept(socket_);
    if (accept_status != sf::Socket::Done) {
      connected_ = false;
      LOG(ERROR) << "Could not receive the socket, status: " << accept_status;
    } else {
      connected_ = true;
    }
  }
}

}  // namespace communication
}  // namespace c24
