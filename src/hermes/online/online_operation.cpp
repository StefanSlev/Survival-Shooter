#include "hermes/online/online_operation.h"

BEGIN_HERMES_NAMESPACE

BEGIN_ONLINE_NAMESPACE

OnlineOperation::OnlineOperation() : m_onlineOperationMethod(OnlineOperationMethod::HttpGet)
{
	this->m_okStatusCode = 200;
}

async::AsyncOperationType OnlineOperation::GetAsyncOperationType() const
{
	return async::AsyncOperationType::OnlineOperation;
}

void OnlineOperation::StartInternal()
{
	cpr::Response response;

	switch (m_onlineOperationMethod)
	{
	case OnlineOperationMethod::HttpGet:
		response = m_session.Get();
		break;
	case OnlineOperationMethod::HttpPost:
		response = m_session.Post();
		break;
	case OnlineOperationMethod::HttpPut:
		response = m_session.Put();
		break;
	case OnlineOperationMethod::HttpDelete:
		response = m_session.Delete();
		break;
	default:
		break;
	}

	if (response.error || response.status_code != m_okStatusCode)
	{
		SetOperationStatus(system::OperationStatus::Failed);
	}
	else
	{
		SetOperationStatus(system::OperationStatus::Succeeded);
		const auto& returnValuePtr = GetAsyncReturnValue().GetReturnValuePtr();

		if (returnValuePtr != nullptr)
		{
			try
			{
				auto data = json::json::parse(response.text);
				data.get_to(*returnValuePtr);
			}
			catch (const json::json::parse_error&)
			{
				SetAsyncReturnValue(async::MakeAsyncReturnValue<json::JsonSerializable>(nullptr));
				SetOperationStatus(system::OperationStatus::Failed);
			}
		}
	}
}

END_ONLINE_NAMESPACE

END_HERMES_NAMESPACE