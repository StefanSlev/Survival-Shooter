#ifndef HERMES_NETWORK
#define HERMES_NETWORK

#include "hermes/network/network_base.h"
// Network Object
#include "hermes/network/network_object_id.h"
#include "hermes/network/network_object_type.h"
#include "hermes/network/network_object_init.h"
#include "hermes/network/network_object.h"
#include "hermes/network/network_object_state_data.h"
#include "hermes/network/network_object_state_controller.h"
#include "hermes/network/network_object_factory.h"
// Master Object
#include "hermes/network/master_object.h"
#include "hermes/network/master_state_controller.h"
// Replica Object
#include "hermes/network/replica_object.h"
#include "hermes/network/replica_state_controller.h"
// Network Message
#include "hermes/network/network_channel_type.h"
#include "hermes/network/network_message_type.h"
#include "hermes/network/network_message.h"
#include "hermes/network/network_event.h"
#include "hermes/network/network_event_type.h"
#include "hermes/network/network_event_data.h"
#include "hermes/network/network_object_state.h"
// Network Managers
#include "hermes/network/network_registry.h"
#include "hermes/network/network_flow_listener.h"
#include "hermes/network/network_flow.h"
#include "hermes/network/network_dispatcher.h"
#include "hermes/network/network_worker.h"

#endif // HERMES_NETWORK