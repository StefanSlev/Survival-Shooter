#ifndef HERMES_ONLINE_OPERATION
#define HERMES_ONLINE_OPERATION

#include "hermes/online/online_base.h"
#include "hermes/async/async_operation.h"
#include "hermes/async/async_operation_type.h"
#include "hermes/online/online_operation_type.h"
#include "hermes/json/json_serializable.h"

BEGIN_HERMES_NAMESPACE

BEGIN_ONLINE_NAMESPACE

enum class OnlineOperationMethod : std::uint8_t
{
	HttpGet,
	HttpPost,
	HttpPut,
	HttpDelete
};

class OnlineOperation : public async::AsyncOperation<json::JsonSerializable>
{
public:
	OnlineOperation();

	async::AsyncOperationType GetAsyncOperationType() const override;
	virtual OnlineOperationType GetOnlineOperationType() const = 0;

protected:
	void StartInternal() override;

	cpr::Session m_session;
	std::int32_t m_okStatusCode;
	OnlineOperationMethod m_onlineOperationMethod;
};

using OnlineOperationPtr = std::unique_ptr<OnlineOperation>;

END_ONLINE_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_ONLINE_OPERATION