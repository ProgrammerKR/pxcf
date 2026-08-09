import pxcf.core

def load(filename):
    """Parse a file containing PXCF data."""
    with open(filename, 'r', encoding='utf-8') as f:
        return pxcf.core.parse_string(f.read())

def loads(s):
    """Parse a string containing PXCF data."""
    return pxcf.core.parse_string(s)

def dump(obj, filename):
    """Serialize a Python object to a PXCF formatted file."""
    with open(filename, 'w', encoding='utf-8') as f:
        f.write(pxcf.core.dump_string(obj))

def dumps(obj):
    """Serialize a Python object to a PXCF formatted string."""
    return pxcf.core.dump_string(obj)

# Alias for compatibility with JSON module naming
parse = loads
