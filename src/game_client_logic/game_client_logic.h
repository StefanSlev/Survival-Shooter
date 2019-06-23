#ifndef GAME_CLIENT_LOGIC
#define GAME_CLIENT_LOGIC

#include "game_client_logic/game_client_logic_base.h"

BEGIN_GCL_NAMESPACE

class GameClientLogic final : public hermes::communication::GameClient, public shared::network::GameObjectsHandler
{
public:
	GameClientLogic(const yojimbo::Address& address,
					hermes::communication::CommunicationAdapter& communicationAdapter);
	~GameClientLogic() override;

private:
	void Update(double dt) final;
	void Render() final;

	void OnNetworkEvent(hermes::network::NetworkEventSPtr networkEvent) final;
	void OnReplicaObjectCreated(std::uint32_t objectType, hermes::network::ReplicaObjectSPtr replicaObject) final;
	void OnReplicaObjectDestroyed(hermes::network::NetworkObjectIdValue objectIdValue) final;
	
	void CreateLocalPlayer();
	void OnGameObjectEnd(shared::gameplay::Object* gameObject) final;

	std::shared_ptr<shared::network::MasterPlayer> m_localPlayer;
	std::unordered_map<hermes::network::NetworkObjectIdValue, hermes::network::ReplicaObjectSPtr> m_replicaObjectsCache;
};

END_GCL_NAMESPACE

#endif // GAME_CLIENT_LOGIC