import os
from setuptools import setup, Extension

# This package uses the C11 PXCF core directly through CPython Extensions
pxcf_extension = Extension(
    name='pxcf.core',
    sources=[
        '../../src/lexer.c',
        '../../src/parser.c',
        '../../src/value.c',
        '../../src/serializer.c',
        '../../src/pxcf.c',
        '../../src/error.c',
        'pxcf/ext.c' # CPython wrapper bindings (to be implemented)
    ],
    include_dirs=['../../include', '../../src'],
    extra_compile_args=['-std=c11']
)

setup(
    name='pxcf',
    version='1.0.0',
    description='ProX Configuration File (PXCF) Parser',
    author='ProgrammerKR',
    packages=['pxcf'],
    ext_modules=[pxcf_extension],
    classifiers=[
        'Programming Language :: Python :: 3',
        'Programming Language :: C',
    ]
)
