#include "plugin/Config.h"

#include "hv/WebSocketClient.h"


#include <string>


namespace WebSocket {

class webSocket {

public:
    hv::WebSocketClient ws;

    void connect(std::string host, std::string password);

    ~webSocket();

    void send(std::string message);
};


}; // namespace WebSocket

namespace {
WebSocket::webSocket ws;
}