from setuptools import setup

requires = [

    'wheel',
    'pyramid',
    'waitress',
    'sqlalchemy',
    'mysql-connector-python-rf',
    'pyjwt',
    'sklearn',
]

dev_requies = [
    'ipdb',
]

setup(
    name='services',
    install_requires = requires,
    extras_require = {
        'dev' : dev_requies
    },
    entry_points={
        'paste.app_factory': [
            'main = services:main'
        ],
    },
)