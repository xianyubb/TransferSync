#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/Event.h"
#include "ll/api/event/ListenerBase.h"


#include "string"

namespace TransferPacketEvent {

class TransferPacketEvent final :public ll::event::Cancellable<ll::event::Event> {
public:
    constexpr explicit TransferPacketEvent(std::string mServerAddress, int mServerPort)
    : mServerAddress(mServerAddress),
      mServerPort(mServerPort){};

    std::string getServerAddress() const { return mServerAddress; }
    int getServerPort() const { return mServerPort; }

    std::string mServerAddress;
    int         mServerPort;
};
}; // namespace lTransferPacketevent
