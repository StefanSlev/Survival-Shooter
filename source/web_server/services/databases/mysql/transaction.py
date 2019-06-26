from enum import Enum
from services.databases.mysql.context import MySqlContext

class ETransactionStatus(Enum):

    # transaction executed with success
    Successful = 0

    # transaction executed and failed
    Failed = 1

    # transaction not yet executed
    Ready = 2

# ------------------------------------------------- 
# A transaction is a self-contained object used
# to execute queries over a SQL database inside a
# session. Uses MySqlContext to create its session
# -------------------------------------------------

class Transaction:

    def __init__(self, verbose = None):

        if verbose is None:
            verbose = False

        # setting sql context for execution
        self._mySqlContext_ = MySqlContext()

        # setting properties of transaction
        self._verbose_ = verbose
        self._status_ = ETransactionStatus.Ready
        self._result_ = None

        # setting properties for execution
        self._callback_ = (lambda db_session: None)
        self._args_ = ()

    # -----------------------------------------
    # -----         Callback header       -----
    # transaction_callback(db_session, *args)
    # -----------------------------------------

    def load_callback(self, callback):
        self._callback_ = callback

    def load_args(self, *args):
        self._args_ = args

    @property
    def status(self):
        return self._status_

    @property
    def result(self):
        return self._result_

    def execute(self):
        
        db_session = self._mySqlContext_.create_session()

        self._status_ = ETransactionStatus.Successful
        self._result_ = None

        try:
            self._result_ = self._callback_(db_session, *self._args_)
            db_session.commit()
        
        except Exception as exception:
            
            if self._verbose_:
                import logging; logging.error(exception)

            self._status_ = ETransactionStatus.Failed
            self._result_ = None

            db_session.rollback()
        finally:
            db_session.close()
    
    @classmethod
    def buildTransaction(cls, callback, *args):
        transaction = cls(verbose=True)
        transaction.load_callback(callback)
        transaction.load_args(*args)
        return transaction