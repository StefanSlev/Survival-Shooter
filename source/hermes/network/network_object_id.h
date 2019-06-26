#ifndef HERMES_NETWORK_OBJECT_ID
#define HERMES_NETWORK_OBJECT_ID

#include "hermes/network/network_base.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

using NetworkObjectIdValue = std::uint64_t;
constexpr NetworkObjectIdValue InvalidNetworkObjectIdValue = UINT64_MAX;

class NetworkObjectId
{
public:
	NetworkObjectId();

	NetworkId GetNetworkId() const;
	ObjectId GetLocalObjectId() const;
	NetworkObjectIdValue GetValue() const;

	void SetValue(NetworkObjectIdValue networkObjectIdValue);
	void SetValue(NetworkId networkId, ObjectId localObjectId);

private:
	NetworkId m_networkId;
	ObjectId m_localObjectId;
};

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_NETWORK_OBJECT_ID
