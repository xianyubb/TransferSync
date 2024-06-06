#include "plugin/Config.h"
#include "plugin/DataStruct/DataStruct.h"

#include <string>

#include "mc/world/actor/player/Player.h"


namespace Form {
void sendSimpleForm(Player& player);

struct CmdParam {
    std::string IP;
    int         Port = 19132;
};

void TransServer(Player& player, CmdParam const& CmdParam);

Motd GetMotd(const Server& server);
} // namespace Form