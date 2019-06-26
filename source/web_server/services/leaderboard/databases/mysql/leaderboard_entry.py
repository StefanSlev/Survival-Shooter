from sqlalchemy import Column, Integer, String
from services.databases.mysql.context import MySqlContext

ContextBase = MySqlContext().get_base()

class LeaderboardEntry(ContextBase):

    __tablename__ = 'leaderboard_entries'

    # columns
    entry_id = Column(Integer, primary_key = True)
    name = Column(String(45))
    score = Column(Integer)

    def __init__(self, name, score):
        self.entry_id = 0
        self.name = name
        self.score = score