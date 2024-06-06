
#include <string>

#include "plugin/WebSocket.h"

#include "plugin/DataStruct/DataStruct.h"


#include "nlohmann/json.hpp"

#include "ll/api/Logger.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/event/player/PlayerChatEvent.h"


void to_json(nlohmann::json& j, const struct ChatMessage& msg) {
    j = nlohmann::json{
        {"Type",       static_cast<int>(msg.Type)},
        {"ServerName", msg.ServerName            },
        {"message",    msg.message               },
        {"playerName", msg.playerName            }
    };
}


namespace Listener {
void ListenChat(WebSocket::webSocket& wss) {
    auto& eventBus = ll::event::EventBus::getInstance();

    eventBus.emplaceListener<ll::event::PlayerChatEvent>([&](ll::event::PlayerChatEvent& ev) {
        auto& player = ev.self();


        struct ChatMessage message {
            {MessageType::ChatMessage, config.Sync.Name}, player.getRealName(), ev.message()
        };

        ll::Logger logger;

        logger.info(message.playerName + ": " + message.message);

        nlohmann::json j       = message;  // 将 Message 结构体转换为 JSON 对象
        std::string    jsonStr = j.dump(); // 将 JSON 对象转换为字符串
        wss.send(jsonStr);
    });
}


} // namespace Listener