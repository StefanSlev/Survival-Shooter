import functools

def synchronized(lock):

    def wrapper(func):
        
        @functools.wraps(func)
        def inner_wrapper(*args, **kwargs):
            with lock:
                return func(*args, **kwargs)
       
        return inner_wrapper
    return wrapper