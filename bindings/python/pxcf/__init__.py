import pxcf.core

def load(filename):
    with open(filename, 'r', encoding='utf-8') as f:
        return pxcf.core.parse_string(f.read())

def loads(s):
    return pxcf.core.parse_string(s)
