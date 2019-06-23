#ifndef HERMES_COMMUNICATION_BASE
#define HERMES_COMMUNICATION_BASE

#include "hermes/hermes_base.h"
#include <yojimbo/yojimbo.h>

BEGIN_HERMES_NAMESPACE

#define BEGIN_COMMUNICATION_NAMESPACE namespace communication {
#define END_COMMUNICATION_NAMESPACE }

BEGIN_COMMUNICATION_NAMESPACE

bool Initialize();
void Shutdown();

using ClientUPtr = std::unique_ptr<yojimbo::Client>;
using ClientSPtr = std::shared_ptr<yojimbo::Client>;

constexpr int MaxMessageSize = 1024;
using CommunicationMessageData = std::uint8_t[MaxMessageSize];

END_COMMUNICATION_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_COMMUNICATION_BASE