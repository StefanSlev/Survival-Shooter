#ifndef SHARED_NETWORK_SYNCHRONIZATION_SERVICE
#define SHARED_NETWORK_SYNCHRONIZATION_SERVICE

#include "shared/network/network_base.h"
#include "shared/gameplay/engine/ids_linker.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class SynchronizationService : public hermes::system::Singleton<SynchronizationService>
{
	SINGLETON_ACCESS(SynchronizationService)

public:
	enum class IDType 
	{
		Local,
		Network
	};

	using LocalToNetworkLinker = gameplay::IDsLinker<int, hermes::network::NetworkObjectIdValue>;
	using NetworkToLocalLinker = gameplay::IDsLinker<hermes::network::NetworkObjectIdValue, int>;

	const LocalToNetworkLinker& LocalToNetwork() const { return localToNetwork; }
	const NetworkToLocalLinker& NetworkToLocal() const { return networkToLocal; }

	void RegisterNetworkObjects();
	void UnregisterNetworkObjects();

	void ClearInfo();

	// returns true if operation was successful, false otherwise
	bool LinkLocalNetworkIds(int localId, hermes::network::NetworkObjectIdValue networkId);
	bool UnlinkLocalId(int id);
	bool UnlinkNetworkId(hermes::network::NetworkObjectIdValue networkId);

	bool IsLinkedToNetwork(int localId);
	bool IsLinkedToLocal(hermes::network::NetworkObjectIdValue networkId);

private:
	LocalToNetworkLinker localToNetwork;
	NetworkToLocalLinker networkToLocal;

	SynchronizationService() {}
	~SynchronizationService() {}
};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#define GetSynchronizationService() shared::network::SynchronizationService::GetInstance()

#endif // SHARED_NETWORK_SYNCHRONIZATION_SERVICE