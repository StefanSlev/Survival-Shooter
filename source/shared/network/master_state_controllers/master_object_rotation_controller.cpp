#include "shared/network/master_state_controllers/master_object_rotation_controller.h"
#include "shared/network/network_objects_state_data/network_object_rotation_data.h"
#include "shared/gameplay/engine/vector2d.h"
#include "shared/gameplay/engine/constants.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

MasterObjectRotationController::MasterObjectRotationController(hermes::network::NetworkObjectStateId networkObjectStateId, float syncThreshold)
	: MasterStateController(networkObjectStateId, syncThreshold)
{
}

float MasterObjectRotationController::ComputeSyncRate(hermes::network::NetworkObjectStateDataSPtr stateData) const
{
	if (stateData)
	{
		auto rotationData = std::static_pointer_cast<NetworkObjectRotationData>(stateData);
		hermes::network::NetworkObjectStateDataSPtr currentStateData = GetCurrentStateData();

		if (currentStateData)
		{
			auto currentRotationData = std::static_pointer_cast<NetworkObjectRotationData>(currentStateData);

			gameplay::Vector2D lookDirection = gameplay::Vector2D(0, 1).Rotate(rotationData->m_rotation);
			gameplay::Vector2D currentLookDirection = gameplay::Vector2D(0, 1).Rotate(currentRotationData->m_rotation);

			float dot = lookDirection.Dot(currentLookDirection);
			float det = lookDirection.Cross(currentLookDirection);
			float syncAngle = atan2f(det, dot) * 180.f / gameplay::M_PI;

			return 1.f - fminf(fabsf(syncAngle), 90.f) / 90.f;
		}
	}

	return 0.f;
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE