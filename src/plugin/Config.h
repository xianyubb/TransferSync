#include <string>
#include <vector>


struct Server {
    std::string Name    = "Server1";
    std::string Address = "127.0.0.1";
    int         Port    = 19132;
};
struct MSServer {
    std::string MotdAddress = "http://127.0.0.1:8080";
    std::string WsAddress   = "ws://127.0.0.1:8081";
    std::string PassWord    = "password";
};

struct Sync {
    std::string Name           = "Sync1";
    bool        Welcome        = true;
    bool        Chat           = true;
    bool        SyncInventory  = true;
    bool        SyncEnderChest = true;
};


struct Config {
    int                 version = 1;
    std::vector<Server> Server  = {
        {"Server1", "127.0.0.1", 19132}
    };
    MSServer MSServer;
    Sync     Sync;
};


namespace {
Config config;
}
