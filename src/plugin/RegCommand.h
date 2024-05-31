

#include "ll/api/command/CommandHandle.h"
#include "ll/api/command/CommandRegistrar.h"

#include "mc/network/packet/TransferPacket.h"
#include "mc/server/commands/CommandOrigin.h"
#include "mc/server/commands/CommandOutput.h"
#include "mc/server/commands/CommandPermissionLevel.h"

#include "mc/world/actor/Actor.h"
#include "mc/world/actor/player/Player.h"


struct CmdParam {
    std::string IP;
    int         Port = 19132;
};

inline void TransServer(Player& player, CmdParam const& CmdParam) {
    TransferPacket packet(CmdParam.IP, CmdParam.Port);
    packet.sendTo(player);
    // TODO: 向 WebSockket Server 发生需要同步的数据
}

inline void RegCommand() {
    auto& commandInstance = ll::command::CommandRegistrar::getInstance();
    auto& cmd             = commandInstance.getOrCreateCommand("trans", "跨服传送", CommandPermissionLevel::Any);
    cmd.alias("tf");
    cmd.overload()

        .execute([](CommandOrigin const& origin, CommandOutput& output) {
            auto* entity = origin.getEntity();
            if (entity == nullptr || !entity->isType(ActorType::Player)) {
                output.error("玩家不存在");
                return;
            }
            // TODO: 执行发送表单策略
        });
    
    cmd.overload<CmdParam>()
        .required("IP")
        .optional("Port")

        .execute([](CommandOrigin const& origin, CommandOutput& output, CmdParam const& param) {
            auto* entity = origin.getEntity();
            if (entity == nullptr || !entity->isType(ActorType::Player)) {
                output.error("玩家不存在");
                return;
            }

            auto* player = static_cast<Player*>(entity);
            TransServer(*player, param);
        });
}