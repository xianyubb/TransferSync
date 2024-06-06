#include "plugin/Form.h"

#include "nlohmann/json.hpp"

#include <string>

#include "hv/requests.h"


#include "ll/api/form/CustomForm.h"
#include "ll/api/form/SimpleForm.h"
#include "mc/network/packet/TransferPacket.h"

namespace Form {

void sendSimpleForm(Player& player) {
    ll::form::SimpleForm form;
    // if (form == nullptr) return;
    form.setTitle("跨服传送");
    form.setContent("请选择要传送的服务器...");


    for (auto const& server : config.Server) {
        form.appendButton(server.Name, [&](Player& player) {
            // 发送 motd 表单
            auto motd = GetMotd(server);

            ll::form::CustomForm motdForm;
            motdForm

                .setTitle(server.Name)
                .appendLabel("服务器: " + server.Name)
                .appendLabel("地址: " + motd.host)
                .appendLabel("状态: " + motd.status)
                .appendLabel("在线人数: " + std::to_string(motd.online))
                .appendLabel("最大人数: " + std::to_string(motd.max))
                .appendLabel("Motd: " + motd.motd)
                .appendLabel("地图名: " + motd.level_name)
                .appendToggle("是否传送到该服务器", "是否传送到该服务器", false)

                .sendTo(
                    player,
                    std::function<void(Player&, ll::form::CustomFormResult const&, ll::form::FormCancelReason)>(
                        [&](Player&                           player,
                            ll::form::CustomFormResult const& CustomFormResult,
                            ll::form::FormCancelReason        FormCancelReasons) {
                            auto& res = std::get<unsigned long long>(CustomFormResult.value().at("是否传送到该服务器"));
                            if (res == 1) {
                                TransServer(player, {server.Address, server.Port});
                                return;
                            }
                            player.sendMessage("取消传送");
                        }
                    )
                );
        });
    }
    form.sendTo(player, {});
};

void TransServer(Player& player, CmdParam const& CmdParam) {
    TransferPacket packet(CmdParam.IP, CmdParam.Port);
    packet.sendTo(player);
    // TODO: 向 WebSockket Server 发生需要同步的数据
};

Motd GetMotd(const Server& server) {
    Motd        motd;
    std::string url = config.MSServer.MotdAddress + "/api?host=" + server.Address + ":" + std::to_string(server.Port);

    auto resp = requests::get(url.c_str());
    if (resp == NULL) return motd;
    auto result = resp->body;

    auto json = nlohmann::json::parse(result);


    if (json == nullptr) return motd;

    motd.status     = json["status"];
    motd.host       = json["host"];
    motd.motd       = json["motd"];
    motd.agreement  = json["agreement"];
    motd.version    = json["version"];
    motd.online     = json["online"];
    motd.max        = json["max"];
    motd.level_name = json["level_name"];
    motd.gamemode   = json["gamemode"];
    motd.delay      = json["delay"];
    return motd;
};

} // namespace Form