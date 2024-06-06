#include "plugin/WebSocket.h"
#include "hv/wsdef.h"


namespace WebSocket {

void webSocket::connect(std::string host, std::string password) { ws.open((host + "/" + password).c_str()); }

webSocket::~webSocket() { ws.close(); }

void webSocket::send(std::string message) { ws.send(message.c_str(),message.length(),WS_OPCODE_BINARY); }
} // namespace WebSocket
