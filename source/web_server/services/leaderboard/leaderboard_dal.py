from services.leaderboard.databases.mysql.leaderboard_entry import LeaderboardEntry
from services.leaderboard.leaderboard_data import LeaderboardEntryData
from services.databases.mysql.transaction import (ETransactionStatus, Transaction)
from sqlalchemy import desc

class LeaderboardDAL:

    def add_entry(self, entry_data):

        transaction = Transaction.buildTransaction(LeaderboardCallback.add_entry, entry_data)
        transaction.execute()

        return transaction.status == ETransactionStatus.Successful

    def get_entries(self):

        transaction = Transaction.buildTransaction(LeaderboardCallback.get_entries)
        transaction.execute()

        if transaction.status == ETransactionStatus.Successful:
            return transaction.result

class LeaderboardCallback:

    @staticmethod
    def add_entry(db_session, entry_data):
        
        entry = LeaderboardEntry(name=entry_data.name, 
                                 score=entry_data.score)
        db_session.add(entry)

    @staticmethod
    def get_entries(db_session):

        entries_limit = 50
        entries = db_session.query(LeaderboardEntry) \
                            .order_by(desc(LeaderboardEntry.score)) \
                            .limit(entries_limit).all()
        return [LeaderboardCallback._convert_to_entry_data_(entry) for entry in entries]

    @staticmethod
    def _convert_to_entry_data_(entry):
        
        return LeaderboardEntryData(name=entry.name,
                                    score=entry.score)