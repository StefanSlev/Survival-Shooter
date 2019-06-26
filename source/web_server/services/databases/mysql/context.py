from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker
from sqlalchemy import engine_from_config
from services.utils.synchronized_singleton import SyncSingleton

# ---------------------------------------
#   Singleton class that keeps the
#   configuration for a SQL database
#   once set (class is thread safe)
# ---------------------------------------

class MySqlContext(metaclass=SyncSingleton):

    def __init__(self):

        self._engine_ = None
        self._dbSessionFactory_ = sessionmaker()
        self._base_ = declarative_base()

    def configure_engine(self, **settings):

        # ------------------------------------
        # create engine with default settings 
        # for multithreading performance
        # ------------------------------------

        self._engine_ = engine_from_config(settings, 
            prefix = 'sqlalchemy.',
            pool_size = 10,
            max_overflow = 20)

        self._dbSessionFactory_.configure(bind=self._engine_)
        self._base_.metadata.bind = self._engine_

    def create_session(self):
        return self._dbSessionFactory_()

    def get_base(self):
        return self._base_

    def get_engine(self):
        return self._engine_