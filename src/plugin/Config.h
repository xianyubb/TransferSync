#pragma once



#include <string>
#include <vector>


struct Server {
    std::string Name    = "Server1";
    std::string Address = "127.0.0.1";
    int         Port    = 19132;
};
struct MotdServer {
    std::string Address  = "ws//127.0.0.1:8080";
    std::string Password = "password";
};

struct Sync {
    bool Welcome        = true;
    bool Chat           = true;
    bool SyncInventory  = true;
    bool SyncEnderChest = true;
};

struct Config {
    int                 version = 1;
    std::vector<Server> Server;
    MotdServer          MotdServer;
    Sync                Sync;
};
