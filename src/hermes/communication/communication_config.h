#ifndef HERMES_COMMUNICATION_CONFIG
#define HERMES_COMMUNICATION_CONFIG

#include "hermes/communication/communication_base.h"

BEGIN_HERMES_NAMESPACE

BEGIN_COMMUNICATION_NAMESPACE

enum class CommunicationChannelType : std::uint8_t
{
	Reliable,
	Unreliable,
};

#define GetCommunicationChannelIndex(communicationChannelType) static_cast<int>(communicationChannelType)

constexpr CommunicationChannelType communicationChannelTypes[] = {
	
	CommunicationChannelType::Reliable,
	CommunicationChannelType::Unreliable
};

struct CommunicationConfig : public yojimbo::ClientServerConfig
{
	CommunicationConfig()
	{
		this->numChannels = 2;
		this->timeout = -1;
		this->clientMemory = 15 * 1024 * 1024;
		this->serverGlobalMemory = 15 * 1024 * 1024;
		this->serverPerClientMemory = 15 * 1024 * 1024;
		this->channel[GetCommunicationChannelIndex(CommunicationChannelType::Reliable)].type = yojimbo::CHANNEL_TYPE_RELIABLE_ORDERED;
		this->channel[GetCommunicationChannelIndex(CommunicationChannelType::Unreliable)].type = yojimbo::CHANNEL_TYPE_UNRELIABLE_UNORDERED;
	}
};

END_COMMUNICATION_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_COMMUNICATION_CONFIG