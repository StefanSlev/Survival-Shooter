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
	void Update(double dt) override;
	void Render() override;

	void OnNetworkEvent(hermes::network::NetworkEventSPtr networkEvent) override;
	void OnReplicaObjectCreated(std::uint32_t objectType, hermes::network::ReplicaObjectSPtr replicaObject) override;
	void OnReplicaObjectDestroyed(hermes::network::NetworkObjectIdValue objectIdValue) override;
	
	void CreateLocalPlayer();
	void OnGameObjectEnd(shared::gameplay::Object* gameObject) override;

	std::shared_ptr<shared::network::MasterPlayer> m_localPlayer;
	std::unordered_map<hermes::network::NetworkObjectIdValue, hermes::network::ReplicaObjectSPtr> m_replicaObjectsCache;
};

END_GCL_NAMESPACE

#endif // GAME_CLIENT_LOGIC