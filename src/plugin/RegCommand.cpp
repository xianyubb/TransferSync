
#include "plugin/RegCommand.h"

#include "plugin/Form.h"

#include <string>

#include "ll/api/command/CommandHandle.h"
#include "ll/api/command/CommandRegistrar.h"

#include "mc/server/commands/CommandOrigin.h"
#include "mc/server/commands/CommandOutput.h"
#include "mc/server/commands/CommandPermissionLevel.h"

#include "mc/world/actor/Actor.h"
#include "mc/world/actor/player/Player.h"


#include "mc/world/actor/player/Player.h"


namespace RegCommand {

void regCommand() {
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
            auto* player = static_cast<Player*>(entity);
            Form::sendSimpleForm(*player);
        });

    cmd.overload<Form::CmdParam>()
        .required("IP")
        .optional("Port")

        .execute([](CommandOrigin const& origin, CommandOutput& output, Form::CmdParam const& param) {
            auto* entity = origin.getEntity();
            if (entity == nullptr || !entity->isType(ActorType::Player)) {
                output.error("玩家不存在");
                return;
            }

            auto* player = static_cast<Player*>(entity);
            TransServer(*player, param);
        });
};
} // namespace RegCommand