#ifndef DEDICATED_GAME_SERVER
#define DEDICATED_GAME_SERVER

#include "dedicated_game_server/dedicated_game_server_base.h"

BEGIN_DGS_NAMESPACE

class DedicatedGameServer final : public hermes::communication::DedicatedServer, public shared::network::GameObjectsHandler
{
public:
	DedicatedGameServer(const uint8_t privateKey[],
						const yojimbo::Address& serverAddress,
						hermes::communication::DedicatedServerAdapter& dedicatedServerAdapter);
	~DedicatedGameServer() override;

private:
	void Update(double dt) override;
	void Render() override;
	
	void OnClientConnected(int clientIndex) override;
	void OnClientDisconnected(int clientIndex) override;
	
	void OnNetworkEvent(hermes::network::NetworkEventSPtr networkEvent) override;
	void OnReplicaObjectCreated(std::uint32_t objectType, hermes::network::ReplicaObjectSPtr replicaObject) override;
	void OnReplicaObjectDestroyed(hermes::network::NetworkObjectIdValue objectIdValue) override;
	
	void AddGameObject(shared::gameplay::ObjectType objectType, hermes::network::NetworkObjectInitDataSPtr objectInitData = nullptr) override;
	void OnGameObjectEnd(shared::gameplay::Object* gameObject) override;

	std::unordered_map<hermes::network::NetworkObjectIdValue, hermes::network::MasterObjectSPtr> m_masterObjectsCache;
	hermes::network::NetworkEventSPtr CreateMasterObjectEvent(hermes::network::MasterObjectSPtr masterObject);

	std::unordered_map<int, std::shared_ptr<shared::network::ReplicaPlayer>> m_replicaPlayersCache;
	hermes::network::NetworkEventSPtr CreateReplicaPlayerEvent(std::shared_ptr<shared::network::ReplicaPlayer> replicaPlayer);
};

END_DGS_NAMESPACE

#endif // DEDICATED_GAME_SERVER