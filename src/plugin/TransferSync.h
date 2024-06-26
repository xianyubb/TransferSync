
#include "RemoteCallAPI.h"
#include "ll/api/Config.h"
#include "ll/api/plugin/NativePlugin.h"


namespace transfer_sync {

class TransferSync {

public:
    static TransferSync& getInstance();

    TransferSync(ll::plugin::NativePlugin& self) : mSelf(self) {}

    [[nodiscard]] ll::plugin::NativePlugin& getSelf() const { return mSelf; }

    /// @return True if the plugin is loaded successfully.
    bool load();

    /// @return True if the plugin is enabled successfully.
    bool enable();

    /// @return True if the plugin is disabled successfully.
    bool disable();

    // TODO: Implement this method if you need to unload the plugin.
    // /// @return True if the plugin is unloaded successfully.
    // bool unload();

private:
    ll::plugin::NativePlugin& mSelf;
};

} // namespace transfer_sync
