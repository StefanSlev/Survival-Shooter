#include "shared/network/master_state_controllers/master_object_position_controller.h"
#include "shared/network/network_objects_state_data/network_object_position_data.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

MasterObjectPositionController::MasterObjectPositionController(hermes::network::NetworkObjectStateId networkObjectStateId, float syncThreshold) 
	: MasterStateController(networkObjectStateId, syncThreshold)
{
}

float MasterObjectPositionController::ComputeSyncRate(hermes::network::NetworkObjectStateDataSPtr stateData) const
{
	if (stateData)
	{
		auto positionData = std::static_pointer_cast<NetworkObjectPositionData>(stateData);
		hermes::network::NetworkObjectStateDataSPtr currentStateData = GetCurrentStateData();

		if (currentStateData)
		{
			auto currentPositionData = std::static_pointer_cast<NetworkObjectPositionData>(currentStateData);
			gameplay::Vector2D diff = positionData->m_position - currentPositionData->m_position;

			return 1.f - fminf(diff.LengthSq(), 1.f);
		}
	}

	return 0.f;
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE