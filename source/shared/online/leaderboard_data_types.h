#ifndef SHARED_ONLINE_LEADERBOARD_DATA_TYPES
#define SHARED_ONLINE_LEADERBOARD_DATA_TYPES

#include "shared/online/online_base.h"

BEGIN_SHARED_NAMESPACE

BEGIN_ONLINE_NAMESPACE

struct LeaderboardEntry final : public hermes::json::JsonSerializable
{
	std::string name;
	std::uint32_t score;

	LeaderboardEntry()
	{
		REGISTER_JSON_FIELD("name", name);
		REGISTER_JSON_FIELD("score", score);
	}
};

struct Leaderboard final : public hermes::json::JsonSerializable
{
	std::vector<LeaderboardEntry> entries;

	Leaderboard()
	{
		REGISTER_JSON_FIELD("entries", entries);
	}
};

END_ONLINE_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_ONLINE_LEADERBOARD_DATA_TYPES