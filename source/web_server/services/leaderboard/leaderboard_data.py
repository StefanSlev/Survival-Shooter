from collections import namedtuple

LeaderboardEntryData = namedtuple("LeaderboardEntryData", ["name", "score"])

# -------------------------------------------------------------------------------
# Convert from json to data
# -------------------------------------------------------------------------------

def _convert_json_to_leaderboard_entry_data_(leaderboard_entry_data_json):

    return LeaderboardEntryData(name = leaderboard_entry_data_json["name"],
                                score = leaderboard_entry_data_json["score"])

_from_json_converter_ = {

    LeaderboardEntryData : _convert_json_to_leaderboard_entry_data_
}

def convert_json_to_info(json, info_class):
    
    try:
        if info_class in _from_json_converter_:
            return _from_json_converter_[info_class](json)
    except:
        return None

# -------------------------------------------------------------------------------
# Convert from data to json
# -------------------------------------------------------------------------------

def _convert_leaderboard_entry_data_to_json(leaderboard_entry_data):

    if isinstance(leaderboard_entry_data, LeaderboardEntryData):
        return dict(leaderboard_entry_data._asdict())

_to_json_converter_ = {
    LeaderboardEntryData : _convert_leaderboard_entry_data_to_json
}

def convert_info_to_json(info):
    info_class = info.__class__

    if info_class in _to_json_converter_:
        return _to_json_converter_[info_class](info)