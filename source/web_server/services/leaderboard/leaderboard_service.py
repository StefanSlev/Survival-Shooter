from json import JSONDecodeError
from pyramid.view import view_config

from services.service_response import (ServiceResponse, ServiceStatus,
                                       ServiceType, build_view_response)

from services.leaderboard.leaderboard_dal import LeaderboardDAL
from services.leaderboard.leaderboard_data import LeaderboardEntryData
from services.leaderboard.leaderboard_data import convert_info_to_json, convert_json_to_info

class LeaderboardService:

    def __init__(self, request):

        self._request_ = request
        self._dal_ = LeaderboardDAL()

    def _get_request_parameter_(self, key):

        parameter = None
        
        try:
            parameter = self._request_.json_body.get(key)
        except Exception:
            parameter = None
        return parameter

    @view_config(route_name = "add_leaderboard_entry", request_method = "POST")
    def add_leaderboard_entry(self):
        
        service_response = ServiceResponse(service_type=ServiceType.ADD_LEADERBOARD_ENTRY)
        name = self._get_request_parameter_('name')
        score = self._get_request_parameter_("score")

        if name and score:
            entry_data = LeaderboardEntryData(name=name, score=score)
            result = self._dal_.add_entry(entry_data)

            service_response.service_status = ServiceStatus.CREATED if result \
                        else ServiceStatus.CONFLICT

        return build_view_response(service_response)

    @view_config(route_name = "get_leaderboard", request_method = "GET")
    def get_leaderboard(self):

        service_response = ServiceResponse(service_type=ServiceType.GET_LEADERBOARD)
        entries_data = self._dal_.get_entries()

        if entries_data is not None:
            service_response.service_status = ServiceStatus.OK
            service_response.add_data('entries', [convert_info_to_json(entry_data) for entry_data in entries_data])
        else:
            service_response.service_status = ServiceStatus.NOT_FOUND

        return build_view_response(service_response)