#include "plugin/TransferSync.h"
#include "plugin/DataStruct/DataStruct.h"
#include "plugin/Listener.h"
#include "plugin/RegCommand.h"
#include "plugin/TransferPacketEvent.h"



#include <memory>
#include <stdexcept>
#include <string>


#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"

#include "ll/api/Logger.h"
#include "ll/api/event/Emitter.h"
#include "ll/api/event/EmitterBase.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/event/ListenerBase.h"
#include "ll/api/memory/Hook.h"
#include "ll/api/plugin/NativePlugin.h"
#include "ll/api/plugin/RegisterHelper.h"


#include "mc/network/packet/TransferPacket.h"

void DealReceiveMessage(const std::string& msg) {
    ll::Logger logger;

    try {
        const auto res = nlohmann::json::parse(msg);
        if (res["ServerName"] == config.Sync.Name) return;
        if (res["Type"] == MessageType::ChatMessage) {
            logger.info(res["Message"]);
        }
    } catch (std::runtime_error) {
        logger.error("json parse error");
        return;
    };
}


namespace TransferPacketEvent {

LL_AUTO_TYPE_INSTANCE_HOOK(
    TransferPacketHook,
    ll::memory::HookPriority::Normal,
    TransferPacket,
    "??0TransferPacket@@QEAA@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@H@Z",
    void,
    std::string ServerAddress, // this+0x30
    int         ServerPort     // this+0x50
) {
    origin(ServerAddress, ServerPort);
    // ll::Logger logger("TransferPackageHook");
    // logger.info("IP is : " + ServerAddress);
    // logger.info("Port is : " + std::to_string(ServerPort));

    auto event = TransferPacketEvent{ServerAddress, ServerPort};
    ll::event::EventBus::getInstance().publish(event);

    if (event.isCancelled()) {
        return;
    }
}
static std::unique_ptr<ll::event::EmitterBase> emitterFactory(ll::event::ListenerBase&);
class TransferPacketEventEmitter : public ll::event::Emitter<emitterFactory, TransferPacketEvent> {
    ll::memory::HookRegistrar<TransferPacketHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory(ll::event::ListenerBase&) {
    return std::make_unique<TransferPacketEventEmitter>();
}
} // namespace TransferPacketEvent

namespace transfer_sync {

static std::unique_ptr<TransferSync> instance;

TransferSync& TransferSync::getInstance() { return *instance; }

void loadconfig() {
    ll::Logger  logger;
    const auto& configFilePath = "./plugins/TransferSync/config.json";
    if (!ll::config::loadConfig(config, configFilePath)) {
        logger.warn("Cannot load configurations from {}", configFilePath);
        logger.info("Saving default configurations");
        if (!ll::config::saveConfig(config, configFilePath)) {
            logger.error("Cannot save default configurations to {}", configFilePath);
        }
    }
}

bool TransferSync::load() {


    getSelf().getLogger().info("Loading...");


    // Load or initialize configurations.
    loadconfig();

    auto eventBus = &ll::event::EventBus::getInstance();
    RemoteCall::exportAs(
        "TransferSync",
        "TransferSyncEvent",
        [eventBus](std::string const& eventName, std::string const& eventId) {
            if (RemoteCall::hasFunc(eventName, eventId)) {
                switch (ll::hash_utils::doHash(eventName)) {
                case ll::hash_utils::doHash("onTransferPacketEvent"):
                    auto Call = RemoteCall::importAs<bool(std::string const& ServerAddress, int const ServerPort)>(
                        eventName,
                        eventId
                    );
                    eventBus->emplaceListener<TransferPacketEvent::TransferPacketEvent>(
                        [Call](TransferPacketEvent::TransferPacketEvent& ev) {
                            try {
                                Call(ev.getServerAddress(), ev.getServerPort());
                            } catch (...) {}
                        }
                    );
                    return true;
                }
            }
            return true;
        }
    );
    // Code for loading the plugin goes here.
    ws.ws.onopen    = []() { printf("onopen\n"); };
    ws.ws.onmessage = [](const std::string& msg) {
        DealReceiveMessage(msg);
        // printf("onmessage: %.*s\n", (int)msg.size(), msg.data());
    };
    ws.ws.onclose = []() { printf("onclose\n"); };
    ws.connect(config.MSServer.WsAddress, config.MSServer.PassWord);
    // ws.send("js");
    Listener::ListenChat(ws);
    return true;
}

bool TransferSync::enable() {
    RegCommand::regCommand();
    // auto& logger   = getSelf().getLogger();


    getSelf().getLogger().info("Enabling...");
    // Code for enabling the plugin goes here.
    return true;
}

bool TransferSync::disable() {
    getSelf().getLogger().info("Disabling...");
    // Code for disabling the plugin goes here.
    return true;
}

// bool TransferSync::unload() {
//     getSelf().getLogger().info("Unloading...");
//     // Code for unloading the plugin goes here.
//     return true;
// }

} // namespace transfer_sync

LL_REGISTER_PLUGIN(transfer_sync::TransferSync, transfer_sync::instance);
