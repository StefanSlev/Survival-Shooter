#ifndef HERMES_NETWORK_OBJECT_FACTORY
#define HERMES_NETWORK_OBJECT_FACTORY

#include "hermes/network/network_base.h"
#include "hermes/network/network_object_id.h"
#include "hermes/network/master_object.h"
#include "hermes/network/replica_object.h"
#include "hermes/system/singleton.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class NetworkObjectFactory : public system::Singleton<NetworkObjectFactory>
{
	SINGLETON_ACCESS(NetworkObjectFactory)

public:
	using MasterObjectFactory = std::function<MasterObjectSPtr(const NetworkObjectId&)>;
	using ReplicaObjectFactory = std::function<ReplicaObjectSPtr(const NetworkObjectId&)>;

	bool RegisterMasterObjectType(std::uint32_t objectType, MasterObjectFactory objectFactory);
	bool UnregisterMasterObjectType(std::uint32_t objectType);

	bool RegisterReplicaObjectType(std::uint32_t objectType, ReplicaObjectFactory objectFactory);
	bool UnregisterReplicaObjectType(std::uint32_t objectType);

	MasterObjectSPtr BuildMasterObject(std::uint32_t objectType, const NetworkObjectId& objectId);
	ReplicaObjectSPtr BuildReplicaObject(std::uint32_t objectType, const NetworkObjectId& objectId);

private:
	std::unordered_map<std::uint32_t, MasterObjectFactory> m_masterObjectFactories;
	std::unordered_map<std::uint32_t, ReplicaObjectFactory> m_replicaObjectFactories;

	NetworkObjectFactory() {}
	~NetworkObjectFactory() override {}
};

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#define GetNetworkObjectFactory() hermes::network::NetworkObjectFactory::GetInstance()

#define BeginMasterObjectFactory(MasterObjectType, masterObject)                                       \
    [](const hermes::network::NetworkObjectId& objectId) -> hermes::network::MasterObjectSPtr          \
    {                                                                                                  \
        std::shared_ptr<MasterObjectType> masterObject = std::make_shared<MasterObjectType>(objectId);

#define EndMasterObjectFactory(MasterObjectType, masterObject)                         \
        return std::static_pointer_cast<hermes::network::MasterObject> (masterObject); \
    }

#define BeginReplicaObjectFactory(ReplicaObjectType, replicaObject)                                       \
    [](const hermes::network::NetworkObjectId& objectId) -> hermes::network::ReplicaObjectSPtr            \
    {                                                                                                     \
        std::shared_ptr<ReplicaObjectType> replicaObject = std::make_shared<ReplicaObjectType>(objectId);

#define EndReplicaObjectFactory(ReplicaObjectType, replicaObject)                        \
        return std::static_pointer_cast<hermes::network::ReplicaObject> (replicaObject); \
    }

#endif // HERMES_NETWORK_OBJECT_FACTORY