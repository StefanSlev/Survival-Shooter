from enum import Enum
from json import dumps, loads, JSONDecodeError
from pyramid.response import Response

class ServiceType(Enum):
    NONE = 'NONE'
    ADD_LEADERBOARD_ENTRY = 'ADD_LEADERBOARD_ENTRY'
    GET_LEADERBOARD = 'GET_LEADERBOARD'
    
class ServiceStatus(Enum):
    OK = 200
    CREATED = 201
    NO_CONTENT = 204
    BAD_REQUEST = 400
    UNAUTHORIZED = 401
    FORBIDDEN = 403
    NOT_FOUND = 404
    METHOD_NOT_ALLOWED = 405
    CONFLICT = 409
    INTERNAL_SERVER_ERROR = 500

class ServiceResponse(Response):
    
    def __init__(self, service_type = None, service_status = None, service_data = None):

        if service_type is None:
            service_type = ServiceType.NONE
        if service_status is None:
            service_status = ServiceStatus.BAD_REQUEST
        if service_data is None:
            service_data = {}

        self.service_type = service_type
        self.service_status = service_status
        self.service_data = service_data

    def add_data(self, key, value):
        self.service_data.update({key : value})

    def serialize(self):
        return dumps(self.service_data)

def build_view_response(service_response):
    return Response(body = service_response.serialize(),
                    status = service_response.service_status.value)