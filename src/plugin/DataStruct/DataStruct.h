
#include <string>


enum MessageType { ChatMessage = 0, PlayerMessage = 1, ServerMessage = 2 };

struct Message {
    /** 消息类型 用于处理对应的数据 避免性能损耗 */
    MessageType Type;
    /** 服务器名称 */
    std::string ServerName;
};

struct ChatMessage : Message {
    /** 发送聊天信息的玩家名 */
    std::string playerName;
    /** 聊天信息 */
    std::string message;
};


struct DropNbtList {
    /** 盔甲栏 */
    std::string Armor;
    /** 背包栏 */
    std::string Inventory;
    /** 副手栏 */
    std::string Offhand;
    /** 光标处与合成栏处等物品NBT */
    std::string PlayerUIItems;
};

struct PlayerMessage : Message {
    /** 玩家名 */
    std::string playerName;
    /** 该玩家xuid */
    std::string playerXuid;
    /** 背包数据 */
    DropNbtList DropNbtList;
    /** 末影箱数据 */
    std::string EnderChestItems;
};


struct Motd {

    std::string status     = "off";             // 服务器状态
    std::string host       = "127.0.0.1:19132"; // 返回查询Host
    std::string motd       = "";                // 服务器广播内容Motd
    int         agreement  = 0;                 // 协议版本
    std::string version    = "0.0.0";           // 客户端版本
    int         online     = 0;                 // 服务器在线人数
    int         max        = 0;                 // 服务器人数上限
    std::string level_name = "";                // 存档名称
    std::string gamemode   = "";                // 游戏模式
    int         delay      = 0;                 // 连接服务器延迟(ms)
};