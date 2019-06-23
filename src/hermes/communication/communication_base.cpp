#include "hermes/communication/communication_base.h"

BEGIN_HERMES_NAMESPACE

BEGIN_COMMUNICATION_NAMESPACE

bool Initialize()
{
	bool status = true;
	
	status &= InitializeYojimbo();
	yojimbo_log_level(YOJIMBO_LOG_LEVEL_INFO);
	
	return status;
}

void Shutdown()
{
	ShutdownYojimbo();
}

END_COMMUNICATION_NAMESPACE

END_HERMES_NAMESPACE