#ifndef SHARED_NETWORK
#define SHARED_NETWORK

#include "shared/network/network_base.h"
#include "shared/network/game_objects_handler.h"
#include "shared/network/game_objects_conversions.h"
#include "shared/network/synchronization_service.h"

#include "shared/network/master_objects/master_player.h"
#include "shared/network/master_objects/master_enemy.h"
#include "shared/network/master_objects/master_kamikaze.h"
#include "shared/network/master_objects/master_undead.h"

#include "shared/network/master_state_controllers/master_object_rotation_controller.h"
#include "shared/network/master_state_controllers/master_object_position_controller.h"
#include "shared/network/master_state_controllers/master_player_rotation_controller.h"
#include "shared/network/master_state_controllers/master_player_position_controller.h"
#include "shared/network/master_state_controllers/master_enemy_rotation_controller.h"
#include "shared/network/master_state_controllers/master_enemy_position_controller.h"
#include "shared/network/master_state_controllers/master_enemy_target_controller.h"

#include "shared/network/network_objects_init_data/player_init_data.h"
#include "shared/network/network_objects_init_data/enemy_init_data.h"

#include "shared/network/network_objects_state_data/network_object_rotation_data.h"
#include "shared/network/network_objects_state_data/network_object_position_data.h"
#include "shared/network/network_objects_state_data/enemy_target_data.h"

#include "shared/network/replica_objects/replica_player.h"
#include "shared/network/replica_objects/replica_enemy.h"
#include "shared/network/replica_objects/replica_kamikaze.h"
#include "shared/network/replica_objects/replica_undead.h"

#include "shared/network/replica_state_controllers/replica_object_rotation_controller.h"
#include "shared/network/replica_state_controllers/replica_object_position_controller.h"
#include "shared/network/replica_state_controllers/replica_player_rotation_controller.h"
#include "shared/network/replica_state_controllers/replica_player_position_controller.h"
#include "shared/network/replica_state_controllers/replica_enemy_rotation_controller.h"
#include "shared/network/replica_state_controllers/replica_enemy_position_controller.h"
#include "shared/network/replica_state_controllers/replica_enemy_target_controller.h"

#endif // SHARED_NETWORK