#ifndef HERMES_COMMUNICATION_ADAPTER
#define HERMES_COMMUNICATION_ADAPTER

#include "hermes/communication/communication_base.h"
#include "hermes/communication/communication_factory.h"

BEGIN_HERMES_NAMESPACE

BEGIN_COMMUNICATION_NAMESPACE

class CommunicationAdapter : public yojimbo::Adapter
{
public:
	yojimbo::MessageFactory * CreateMessageFactory(yojimbo::Allocator & allocator) override
	{
		return YOJIMBO_NEW(allocator, CommunicationFactory, allocator);
	}
};

END_COMMUNICATION_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_COMMUNICATION_ADAPTER