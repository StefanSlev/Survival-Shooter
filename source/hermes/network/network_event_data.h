#ifndef HERMES_NETWORK_EVENT_DATA
#define HERMES_NETWORK_EVENT_DATA

#include "hermes/network/network_base.h"
#include "hermes/network/network_object_id.h"
#include "hermes/network/network_object_init.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

using NetworkEventData = yojimbo::Serializable;
using NetworkFlowEventData = NetworkEventData;

class NetworkFlowInitializeEventData final : public NetworkEventData
{
public:
	std::uint32_t GetNetworkId();
	void SetNetworkId(std::uint32_t networkId);

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()

private:
	template <class Stream> bool Serialize(Stream& stream)
	{
		serialize_uint32(stream, m_networkId);
		return true;
	}

	std::uint32_t m_networkId;
};

class CreateReplicaObjectEventData final : public NetworkEventData
{
public:
	std::uint32_t GetObjectType() const;
	void SetObjectType(std::uint32_t objectType);

	NetworkObjectIdValue GetNetworkObjectIdValue() const;
	void SetNetworkObjectIdValue(NetworkObjectIdValue objectIdValue);

	bool HasNetworkObjectInitData() const;
	void SetHasNetworkObjectInitData(bool hasNetworkObjectInitData);

	NetworkObjectInitWrapper& GetNetworkObjectInitWrapper();

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()

private:
	template <class Stream> bool Serialize(Stream& stream)
	{
		serialize_uint32(stream, m_objectType);
		serialize_uint64(stream, m_networkObjectIdValue);
		serialize_bool(stream, m_hasNetworkObjectInitData);
		m_networkObjectInitWrapper.SerializeInternal(stream);
		return true;
	}

	std::uint32_t m_objectType;
	NetworkObjectIdValue m_networkObjectIdValue;
	bool m_hasNetworkObjectInitData;
	NetworkObjectInitWrapper m_networkObjectInitWrapper;
};

class DestroyReplicaObjectEventData final : public NetworkEventData
{
public:
	NetworkObjectIdValue GetNetworkObjectIdValue() const;
	void SetNetworkObjectIdValue(NetworkObjectIdValue objectIdValue);

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()

private:
	template <class Stream> bool Serialize(Stream& stream)
	{
		serialize_uint64(stream, m_networkObjectIdValue);
		return true;
	}

	NetworkObjectIdValue m_networkObjectIdValue;
};

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_NETWORK_EVENT_DATA
