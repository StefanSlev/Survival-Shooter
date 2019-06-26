#include "shared/online/leaderboard_operations.h"

BEGIN_SHARED_NAMESPACE

BEGIN_ONLINE_NAMESPACE

GetLeaderboardOperation::GetLeaderboardOperation(const std::string& servicePath)
{
	m_session.SetUrl({ servicePath + "/get_leaderboard" });
	m_session.SetHeader({ {"Content-Type", "application/json"} });

	m_onlineOperationMethod = hermes::online::OnlineOperationMethod::HttpGet;
	m_okStatusCode = 200;

	auto leaderboardPtr = hermes::system::MakeReturnValuePtr<Leaderboard>();
	auto serializablePtr = std::static_pointer_cast<hermes::json::JsonSerializable> (leaderboardPtr);

	SetAsyncReturnValue(hermes::async::MakeAsyncReturnValue(serializablePtr));
}

hermes::online::OnlineOperationType GetLeaderboardOperation::GetOnlineOperationType() const
{
	return hermes::online::OnlineOperationType::GetLeaderboard;
}

AddLeaderboardEntryOperation::AddLeaderboardEntryOperation(const std::string& servicePath, const LeaderboardEntry& entry)
{
	hermes::json::json data = entry;

	m_session.SetUrl({ servicePath + "/add_leaderboard_entry" });
	m_session.SetHeader({ {"Content-Type", "application/json"} });
	m_session.SetBody(cpr::Body{ data.dump() });

	m_onlineOperationMethod = hermes::online::OnlineOperationMethod::HttpPost;
	m_okStatusCode = 201;
}

hermes::online::OnlineOperationType AddLeaderboardEntryOperation::GetOnlineOperationType() const
{
	return hermes::online::OnlineOperationType::AddLeaderboardEntry;
}

END_ONLINE_NAMESPACE

END_SHARED_NAMESPACE