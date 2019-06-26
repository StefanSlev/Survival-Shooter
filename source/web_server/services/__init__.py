from pyramid.config import Configurator
from services.databases.mysql.context import MySqlContext

# "The Fairytale Machine" server entry point
def main(global_config, **settings):
    
    # configure mysql database
    mySqlContext = MySqlContext()
    mySqlContext.configure_engine(**settings)
    
    # configure wsgi
    config = Configurator(settings=settings)

    # add url routes
    config.add_route("add_leaderboard_entry", "/add_leaderboard_entry")
    config.add_route("get_leaderboard", "/get_leaderboard")
    config.scan(".leaderboard.leaderboard_service")
   
    return config.make_wsgi_app()