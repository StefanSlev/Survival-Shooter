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
	void Update(double dt) final;
	void Render() final;
	
	void OnClientConnected(int clientIndex) final;
	void OnClientDisconnected(int clientIndex) final;
	
	void OnNetworkEvent(hermes::network::NetworkEventSPtr networkEvent) final;
	void OnReplicaObjectCreated(std::uint32_t objectType, hermes::network::ReplicaObjectSPtr replicaObject) final;
	void OnReplicaObjectDestroyed(hermes::network::NetworkObjectIdValue objectIdValue) final;
	
	void AddGameObject(shared::gameplay::ObjectType objectType, hermes::network::NetworkObjectInitDataSPtr objectInitData = nullptr) final;
	void OnGameObjectEnd(shared::gameplay::Object* gameObject) final;

	std::unordered_map<hermes::network::NetworkObjectIdValue, hermes::network::MasterObjectSPtr> m_masterObjectsCache;
	std::unordered_map<int, std::shared_ptr<shared::network::ReplicaPlayer>> m_replicaPlayersCache;
};

END_DGS_NAMESPACE

#endif // DEDICATED_GAME_SERVER