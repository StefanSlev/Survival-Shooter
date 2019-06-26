#ifndef HERMES_COMMUNICATION_FACTORY
#define HERMES_COMMUNICATION_FACTORY

#include "hermes/communication/communication_base.h"
#include "hermes/communication/communication_message.h"

BEGIN_HERMES_NAMESPACE

BEGIN_COMMUNICATION_NAMESPACE

YOJIMBO_MESSAGE_FACTORY_START(CommunicationFactory, 1)
YOJIMBO_DECLARE_MESSAGE_TYPE(GetCommunicationMessageIndex(CommunicationMessageType::CommunicationMessage), CommunicationMessage)
YOJIMBO_MESSAGE_FACTORY_FINISH()

END_COMMUNICATION_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_COMMUNICATION_FACTORY