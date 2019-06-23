#ifndef SHARED_GAMEPLAY
#define SHARED_GAMEPLAY

#include "shared/gameplay/gameplay_base.h"

// engine
#include "shared/gameplay/engine/camera.h"
#include "shared/gameplay/engine/collider.h"
#include "shared/gameplay/engine/constants.h"
#include "shared/gameplay/engine/controller.h"
#include "shared/gameplay/engine/engine.h"
#include "shared/gameplay/engine/event.h"
#include "shared/gameplay/engine/font.h"
#include "shared/gameplay/engine/input.h"
#include "shared/gameplay/engine/ids_linker.h"
#include "shared/gameplay/engine/menu_item.h"
#include "shared/gameplay/engine/object.h"
#include "shared/gameplay/engine/object_pools.h"
#include "shared/gameplay/engine/observer.h"
#include "shared/gameplay/engine/physics_object.h"
#include "shared/gameplay/engine/resource_manager.h"
#include "shared/gameplay/engine/rigidbody.h"
#include "shared/gameplay/engine/scrollable_list.h"
#include "shared/gameplay/engine/sprite.h"
#include "shared/gameplay/engine/sprite_animation_controller.h"
#include "shared/gameplay/engine/subject.h"
#include "shared/gameplay/engine/transform2d.h"
#include "shared/gameplay/engine/update_list.h"
#include "shared/gameplay/engine/utils.h"
#include "shared/gameplay/engine/vector2d.h"

// elements
#include "shared/gameplay/elements/actors/actor.h"
#include "shared/gameplay/elements/actors/player.h"
#include "shared/gameplay/elements/actors/enemy.h"

#include "shared/gameplay/elements/animations/animation_controller.h"
#include "shared/gameplay/elements/animations/camera_controller.h"
#include "shared/gameplay/elements/animations/fade_animation.h"
#include "shared/gameplay/elements/animations/move_controller.h"
#include "shared/gameplay/elements/animations/transform_animation.h"

#include "shared/gameplay/elements/bullets/bullet.h"
#include "shared/gameplay/elements/bullets/gauss_bullet.h"
#include "shared/gameplay/elements/bullets/rocket.h"

#include "shared/gameplay/elements/fx/bullet_explosion.h"
#include "shared/gameplay/elements/fx/explosion.h"
#include "shared/gameplay/elements/fx/nuke_explosion.h"
#include "shared/gameplay/elements/fx/rocket_explosion.h"
#include "shared/gameplay/elements/fx/splatter.h"

#include "shared/gameplay/elements/game_events/game_event.h"
#include "shared/gameplay/elements/game_events/game_events_queue.h"

#include "shared/gameplay/elements/game_utilities/game_input.h"
#include "shared/gameplay/elements/game_utilities/game_utils.h"
#include "shared/gameplay/elements/game_utilities/collision_handler.h"
#include "shared/gameplay/elements/game_utilities/spatial_grid.h"

#include "shared/gameplay/elements/menu_pages/menu_game_load.h"
#include "shared/gameplay/elements/menu_pages/player_hud.h"
#include "shared/gameplay/elements/menu_pages/menu_player_hud.h"
#include "shared/gameplay/elements/menu_pages/game_hud.h"

#include "shared/gameplay/elements/power_ups/power_up.h"
#include "shared/gameplay/elements/power_ups/power_up_controller.h"
#include "shared/gameplay/elements/power_ups/power_up_info.h"
#include "shared/gameplay/elements/power_ups/power_up_manager.h"
#include "shared/gameplay/elements/power_ups/power_up_utils.h"

#include "shared/gameplay/elements/utilities/countdown_clock.h"
#include "shared/gameplay/elements/utilities/countdown_timer.h"
#include "shared/gameplay/elements/utilities/crosshair.h"
#include "shared/gameplay/elements/utilities/lifebar.h"

#include "shared/gameplay/elements/weapons/weapon.h"
#include "shared/gameplay/elements/weapons/weapon_info.h"
#include "shared/gameplay/elements/weapons/weapon_info_manager.h"

#include "shared/gameplay/elements/terrain.h"
#include "shared/gameplay/elements/game.h"

#endif // SHARED_GAMEPLAY