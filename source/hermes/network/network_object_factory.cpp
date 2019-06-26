#include "hermes/network/network_object_factory.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

bool NetworkObjectFactory::RegisterMasterObjectType(std::uint32_t objectType, MasterObjectFactory objectFactory)
{
	if (!objectFactory)
		return false;

	auto it = m_masterObjectFactories.find(objectType);

	if (it == m_masterObjectFactories.end())
	{
		m_masterObjectFactories.emplace(objectType, objectFactory);
		return true;
	}
	
	return false;
}

bool NetworkObjectFactory::UnregisterMasterObjectType(std::uint32_t objectType)
{
	auto it = m_masterObjectFactories.find(objectType);

	if (it != m_masterObjectFactories.end())
	{
		m_masterObjectFactories.erase(it);
		return true;
	}

	return false;
}

bool NetworkObjectFactory::RegisterReplicaObjectType(std::uint32_t objectType, ReplicaObjectFactory objectFactory)
{
	if (!objectFactory)
		return false;

	auto it = m_replicaObjectFactories.find(objectType);

	if (it == m_replicaObjectFactories.end())
	{
		m_replicaObjectFactories.emplace(objectType, objectFactory);
		return true;
	}

	return false;
}

bool NetworkObjectFactory::UnregisterReplicaObjectType(std::uint32_t objectType)
{
	auto it = m_replicaObjectFactories.find(objectType);

	if (it != m_replicaObjectFactories.end())
	{
		m_replicaObjectFactories.erase(it);
		return true;
	}

	return false;
}

MasterObjectSPtr NetworkObjectFactory::BuildMasterObject(std::uint32_t objectType, const NetworkObjectId& objectId)
{
	auto it = m_masterObjectFactories.find(objectType);

	if (it != m_masterObjectFactories.end())
	{
		MasterObjectFactory& objectFactory = it->second;
		return objectFactory(objectId);
	}

	return nullptr;
}

ReplicaObjectSPtr NetworkObjectFactory::BuildReplicaObject(std::uint32_t objectType, const NetworkObjectId& objectId)
{
	auto it = m_replicaObjectFactories.find(objectType);

	if (it != m_replicaObjectFactories.end())
	{
		ReplicaObjectFactory& objectFactory = it->second;
		return objectFactory(objectId);
	}

	return nullptr;
}

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE