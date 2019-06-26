#include "shared/network/synchronization_service.h"
#include "shared/network/game_objects_conversions.h"

#include "shared/network/master_objects/master_player.h"
#include "shared/network/master_objects/master_kamikaze.h"
#include "shared/network/master_objects/master_undead.h"
#include "shared/network/replica_objects/replica_player.h"
#include "shared/network/replica_objects/replica_kamikaze.h"
#include "shared/network/replica_objects/replica_undead.h"
#include "shared/network/master_state_controllers/master_player_rotation_controller.h"
#include "shared/network/master_state_controllers/master_enemy_rotation_controller.h"
#include "shared/network/replica_state_controllers/replica_player_rotation_controller.h"
#include "shared/network/replica_state_controllers/replica_enemy_rotation_controller.h"
#include "shared/network/master_state_controllers/master_player_position_controller.h"
#include "shared/network/master_state_controllers/master_enemy_position_controller.h"
#include "shared/network/replica_state_controllers/replica_player_position_controller.h"
#include "shared/network/replica_state_controllers/replica_enemy_position_controller.h"
#include "shared/network/master_state_controllers/master_enemy_target_controller.h"
#include "shared/network/replica_state_controllers/replica_enemy_target_controller.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

void SynchronizationService::ClearInfo()
{
	localToNetwork.ClearLinker();
	networkToLocal.ClearLinker();
}

void SynchronizationService::RegisterNetworkObjects()
{
	GetNetworkObjectFactory().RegisterMasterObjectType
	(
		GetObjectTypeIndex(gameplay::ObjectType::Player),

		BeginMasterObjectFactory(MasterPlayer, masterPlayer)
			masterPlayer->RegisterStateController<MasterPlayerRotationController>(0, 0.90f);
			masterPlayer->RegisterStateController<MasterPlayerPositionController>(1, 0.85f);
		EndMasterObjectFactory(MasterPlayer, masterPlayer)
	);

	GetNetworkObjectFactory().RegisterReplicaObjectType
	(
		GetObjectTypeIndex(gameplay::ObjectType::Player),

		BeginReplicaObjectFactory(ReplicaPlayer, replicaPlayer)
			replicaPlayer->RegisterStateController<ReplicaPlayerRotationController>(0, 1);
			replicaPlayer->RegisterStateController<ReplicaPlayerPositionController>(1, 1);
		EndReplicaObjectFactory(ReplicaPlayer, replicaPlayer)
	);

	GetNetworkObjectFactory().RegisterMasterObjectType
	(
		GetObjectTypeIndex(gameplay::ObjectType::Kamikaze),

		BeginMasterObjectFactory(MasterKamikaze, masterKamikaze)
			masterKamikaze->RegisterStateController<MasterEnemyRotationController>(0, 0.90f);
			masterKamikaze->RegisterStateController<MasterEnemyPositionController>(1, 0.85f);
			masterKamikaze->RegisterStateController<MasterEnemyTargetController>(2);
		EndMasterObjectFactory(MasterKamikaze, masterKamikaze)
	);

	GetNetworkObjectFactory().RegisterReplicaObjectType
	(
		GetObjectTypeIndex(gameplay::ObjectType::Kamikaze),

		BeginReplicaObjectFactory(ReplicaKamikaze, replicaKamikaze)
			replicaKamikaze->RegisterStateController<ReplicaEnemyRotationController>(0, 1);
			replicaKamikaze->RegisterStateController<ReplicaEnemyPositionController>(1, 1);
			replicaKamikaze->RegisterStateController<ReplicaEnemyTargetController>(2, 1);
		EndReplicaObjectFactory(ReplicaKamikaze, replicaKamikaze)
	);

	GetNetworkObjectFactory().RegisterMasterObjectType
	(
		GetObjectTypeIndex(gameplay::ObjectType::Undead),

		BeginMasterObjectFactory(MasterUndead, masterUndead)
			masterUndead->RegisterStateController<MasterEnemyRotationController>(0, 0.90f);
			masterUndead->RegisterStateController<MasterEnemyPositionController>(1, 0.85f);
			masterUndead->RegisterStateController<MasterEnemyTargetController>(2);
		EndMasterObjectFactory(MasterUndead, masterUndead)
	);

	GetNetworkObjectFactory().RegisterReplicaObjectType
	(
		GetObjectTypeIndex(gameplay::ObjectType::Undead),

		BeginReplicaObjectFactory(ReplicaUndead, replicaUndead)
			replicaUndead->RegisterStateController<ReplicaEnemyRotationController>(0, 1);
			replicaUndead->RegisterStateController<ReplicaEnemyPositionController>(1, 1);
			replicaUndead->RegisterStateController<ReplicaEnemyTargetController>(2, 1);
		EndReplicaObjectFactory(ReplicaUndead, replicaUndead)
	);

	GetGameObjectsConversions().RegisterGameObjectConversion<MasterPlayer>
	(
		hermes::network::NetworkObjectType::Master,
		gameplay::ObjectType::Player
	);

	GetGameObjectsConversions().RegisterGameObjectConversion<ReplicaPlayer>
	(
		hermes::network::NetworkObjectType::Replica,
		gameplay::ObjectType::Player
	);

	GetGameObjectsConversions().RegisterGameObjectConversion<MasterKamikaze>
	(
		hermes::network::NetworkObjectType::Master,
		gameplay::ObjectType::Kamikaze
	);

	GetGameObjectsConversions().RegisterGameObjectConversion<ReplicaKamikaze>
	(
		hermes::network::NetworkObjectType::Replica,
		gameplay::ObjectType::Kamikaze
	);

	GetGameObjectsConversions().RegisterGameObjectConversion<MasterUndead>
	(
		hermes::network::NetworkObjectType::Master,
		gameplay::ObjectType::Undead
	);

	GetGameObjectsConversions().RegisterGameObjectConversion<ReplicaUndead>
	(
		hermes::network::NetworkObjectType::Replica,
		gameplay::ObjectType::Undead
	);
}

void SynchronizationService::UnregisterNetworkObjects()
{
	GetNetworkObjectFactory().UnregisterMasterObjectType
	(
		GetObjectTypeIndex(gameplay::ObjectType::Player)
	);

	GetNetworkObjectFactory().UnregisterReplicaObjectType
	(
		GetObjectTypeIndex(gameplay::ObjectType::Player)
	);

	GetNetworkObjectFactory().UnregisterMasterObjectType
	(
		GetObjectTypeIndex(gameplay::ObjectType::Kamikaze)
	);

	GetNetworkObjectFactory().UnregisterReplicaObjectType
	(
		GetObjectTypeIndex(gameplay::ObjectType::Kamikaze)
	);

	GetNetworkObjectFactory().UnregisterMasterObjectType
	(
		GetObjectTypeIndex(gameplay::ObjectType::Undead)
	);

	GetNetworkObjectFactory().UnregisterReplicaObjectType
	(
		GetObjectTypeIndex(gameplay::ObjectType::Undead)
	);

	GetGameObjectsConversions().UnregisterGameObjectConversion
	(
		hermes::network::NetworkObjectType::Master,
		gameplay::ObjectType::Player
	);

	GetGameObjectsConversions().UnregisterGameObjectConversion
	(
		hermes::network::NetworkObjectType::Replica,
		gameplay::ObjectType::Player
	);

	GetGameObjectsConversions().UnregisterGameObjectConversion
	(
		hermes::network::NetworkObjectType::Master,
		gameplay::ObjectType::Kamikaze
	);

	GetGameObjectsConversions().UnregisterGameObjectConversion
	(
		hermes::network::NetworkObjectType::Replica,
		gameplay::ObjectType::Kamikaze
	);

	GetGameObjectsConversions().UnregisterGameObjectConversion
	(
		hermes::network::NetworkObjectType::Master,
		gameplay::ObjectType::Undead
	);

	GetGameObjectsConversions().UnregisterGameObjectConversion
	(
		hermes::network::NetworkObjectType::Replica,
		gameplay::ObjectType::Undead
	);
}

bool SynchronizationService::LinkLocalNetworkIds(int localId, hermes::network::NetworkObjectIdValue networkId)
{
	bool ltonOk = localToNetwork.RegisterForIdLinkage(localId, networkId);
	bool ntolOk = networkToLocal.RegisterForIdLinkage(networkId, localId);

	if (!ltonOk || !ntolOk)
	{
		if (ltonOk)
			localToNetwork.RemoveFromIdLinkage(localId);
		if (ntolOk)
			networkToLocal.RemoveFromIdLinkage(networkId);
	}
	
	return ltonOk && ntolOk;
}

bool SynchronizationService::UnlinkLocalId(int localId)
{
	hermes::network::NetworkObjectIdValue networkId;
	bool linked = localToNetwork.GetLinkedId(localId, networkId);

	if (linked)
	{
		localToNetwork.RemoveFromIdLinkage(localId);
		networkToLocal.RemoveFromIdLinkage(networkId);
	}

	return linked;
}

bool SynchronizationService::UnlinkNetworkId(hermes::network::NetworkObjectIdValue networkId)
{
	int localId;
	bool linked = networkToLocal.GetLinkedId(networkId, localId);

	if (linked)
	{
		localToNetwork.RemoveFromIdLinkage(localId);
		networkToLocal.RemoveFromIdLinkage(networkId);
	}

	return linked;
}

bool SynchronizationService::IsLinkedToNetwork(int localId)
{
	hermes::network::NetworkObjectIdValue networkId;
	return localToNetwork.GetLinkedId(localId, networkId);
}

bool SynchronizationService::IsLinkedToLocal(hermes::network::NetworkObjectIdValue networkId)
{
	int localId;
	return networkToLocal.GetLinkedId(networkId, localId);
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE