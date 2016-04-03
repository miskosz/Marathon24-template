#include "c24/communication/stream_tcp.h"

#include <glog/logging.h>

namespace c24 {
namespace communication {

bool StreamTcp::MessageAvailable() {
  // Check if there is some message in the queue already.
  if (!complete_messages_.empty()) {
    return true;
  }
  if (socket_.isBlocking()) {
    socket_.setBlocking(false);
  }
  std::size_t received;
  sf::TcpSocket::Status status = GetData(&received);
  if (status == sf::TcpSocket::Error || status == sf::TcpSocket::Disconnected) {
    LOG(ERROR) << "TcpSocket error. Status code: " << status;
  }
  // if status == sf::TcpSocket::NotReady no message is yet available.
  return status == sf::TcpSocket::Done;
}

std::string StreamTcp::GetMessage() {
  if (complete_messages_.empty()) {
    if (!socket_.isBlocking()) {
      socket_.setBlocking(true);
    }
    // More than one read may be necessary if the message is longer than
    // _TCP_BUFFER_SIZE.
    while (complete_messages_.empty()) {
      std::size_t received;
      sf::TcpSocket::Status status = GetData(&received);
      if (status == sf::TcpSocket::NotReady || status == sf::TcpSocket::Error ||
          status == sf::TcpSocket::Disconnected) {
        LOG(ERROR)
            << "TcpSocket error when trying to receive data. Status code: "
            << status;
        usleep(500*1000);
        continue;
      }
      if (received == 0) {
        LOG(ERROR) << "Trying to read in blocking mode but no data received";
      }
    }
  }
  std::string msg = complete_messages_.front();
  complete_messages_.pop();
  return msg;
}

bool StreamTcp::SendMessage(const std::string& msg) {
  // Set the socket to blocking mode to guarantee the whole message is sent.
  if (!socket_.isBlocking()) {
    socket_.setBlocking(true);
  }
  sf::TcpSocket::Status status = socket_.send(msg.c_str(), msg.length());
  if (status != sf::TcpSocket::Done) {
    if (status == sf::TcpSocket::Disconnected) {
      connected_ = false;
    }
    LOG(ERROR) << "TcpSocket error. Status code: " << status;
  }
  return status == sf::TcpSocket::Done;
}

// Internally the class remembers queue of received messages. Every time some
// data is received it is stored in buffer_[] and the received data is
// processed. When newline character is found, message is saved to queue
// complete_messages_. Last not complete message (not yet terminated by newline)
// is saved in not_complete_message_.
sf::TcpSocket::Status StreamTcp::GetData(std::size_t* received) {
  sf::TcpSocket::Status status =
      socket_.receive(buffer_, _TCP_BUFFER_SIZE, *received);
  if (status == sf::TcpSocket::Disconnected) {
    connected_ = false;
  }
  if (status == sf::TcpSocket::Done) {
    for (std::size_t i = 0; i < *received; ++i) {
      if (buffer_[i] == '\n') {
        complete_messages_.push(not_complete_message_);
        not_complete_message_ = "";
      } else {
        not_complete_message_.push_back(buffer_[i]);
      }
    }
  }
  return status;
}

}  // namespace communication
}  // namespace c24
