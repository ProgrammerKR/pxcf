#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "pxcf/pxcf.h"

// Forward declaration
static PyObject* pxcf_value_to_py(PxcfValue* val);

static PyObject* pxcf_array_to_py(PxcfValue* val) {
    size_t length = pxcf_array_size(val);
    PyObject* py_list = PyList_New(length);
    for (size_t i = 0; i < length; i++) {
        PxcfValue* elem = pxcf_array_get(val, i);
        PyObject* py_elem = pxcf_value_to_py(elem);
        if (!py_elem) py_elem = Py_None; // Fallback
        PyList_SetItem(py_list, i, py_elem); // Steals reference
    }
    return py_list;
}

static PyObject* pxcf_object_to_py(PxcfValue* val) {
    PyObject* py_dict = PyDict_New();
    if (!py_dict) return NULL;
    
    size_t size = pxcf_object_size(val);
    for (size_t i = 0; i < size; i++) {
        const char* key = NULL;
        PxcfValue* elem = pxcf_object_get_index(val, i, &key);
        if (key && elem) {
            PyObject* py_key = PyUnicode_FromString(key);
            PyObject* py_val = pxcf_value_to_py(elem);
            if (!py_val) py_val = Py_None; // Fallback
            PyDict_SetItem(py_dict, py_key, py_val);
            Py_DECREF(py_key);
            if (py_val != Py_None) Py_DECREF(py_val);
        }
    }
    
    return py_dict;
}

static PyObject* pxcf_value_to_py(PxcfValue* val) {
    if (!val) return Py_None;
    
    switch (pxcf_value_type(val)) {
        case PXCF_VALUE_NULL:
            Py_RETURN_NONE;
        case PXCF_VALUE_BOOL: {
            bool b = false;
            pxcf_value_get_bool(val, &b);
            return PyBool_FromLong(b);
        }
        case PXCF_VALUE_INTEGER: {
            int64_t i = 0;
            pxcf_value_get_integer(val, &i);
            return PyLong_FromLongLong(i);
        }
        case PXCF_VALUE_FLOAT: {
            double d = 0.0;
            pxcf_value_get_float(val, &d);
            return PyFloat_FromDouble(d);
        }
        case PXCF_VALUE_STRING: {
            size_t len = 0;
            const char* s = pxcf_value_get_string(val, &len);
            return PyUnicode_FromStringAndSize(s, len);
        }
        case PXCF_VALUE_ARRAY:
            return pxcf_array_to_py(val);
        case PXCF_VALUE_OBJECT:
            return pxcf_object_to_py(val);
        default:
            Py_RETURN_NONE;
    }
}

static PxcfValue* py_to_pxcf_value(PyObject* obj) {
    if (obj == Py_None) {
        return pxcf_value_new_null();
    } else if (PyBool_Check(obj)) {
        return pxcf_value_new_bool(obj == Py_True);
    } else if (PyLong_Check(obj)) {
        return pxcf_value_new_integer(PyLong_AsLongLong(obj));
    } else if (PyFloat_Check(obj)) {
        return pxcf_value_new_float(PyFloat_AsDouble(obj));
    } else if (PyUnicode_Check(obj)) {
        Py_ssize_t size;
        const char* s = PyUnicode_AsUTF8AndSize(obj, &size);
        if (s) {
            return pxcf_value_new_string(s, (size_t)size);
        }
    } else if (PyList_Check(obj)) {
        PxcfValue* arr = pxcf_value_new_array();
        Py_ssize_t size = PyList_Size(obj);
        for (Py_ssize_t i = 0; i < size; i++) {
            PyObject* item = PyList_GetItem(obj, i); // borrowed ref
            PxcfValue* pxcf_item = py_to_pxcf_value(item);
            if (pxcf_item) {
                pxcf_array_append(arr, pxcf_item);
            }
        }
        return arr;
    } else if (PyDict_Check(obj)) {
        PxcfValue* dict = pxcf_value_new_object();
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        while (PyDict_Next(obj, &pos, &key, &value)) {
            if (PyUnicode_Check(key)) {
                const char* key_str = PyUnicode_AsUTF8(key);
                PxcfValue* pxcf_val = py_to_pxcf_value(value);
                if (key_str && pxcf_val) {
                    pxcf_object_set(dict, key_str, pxcf_val);
                }
            }
        }
        return dict;
    }
    return pxcf_value_new_null(); // Fallback
}

static PyObject* parse_string(PyObject* self, PyObject* args) {
    const char* source;
    Py_ssize_t length;
    
    if (!PyArg_ParseTuple(args, "s#", &source, &length)) {
        return NULL;
    }
    
    PxcfError err;
    PxcfDocument* doc = pxcf_parse_string(source, (size_t)length, &err);
    if (!doc) {
        PyErr_Format(PyExc_SyntaxError, "PXCF Error %d at line %d, col %d: %s", 
                     err.code, err.line, err.column, err.message);
        return NULL;
    }
    
    PyObject* result = pxcf_value_to_py(pxcf_document_root(doc));
    pxcf_document_free(doc);
    return result;
}

static PyObject* dump_string(PyObject* self, PyObject* args) {
    PyObject* py_dict;
    
    if (!PyArg_ParseTuple(args, "O", &py_dict)) {
        return NULL;
    }
    
    PxcfValue* val = py_to_pxcf_value(py_dict);
    if (!val) {
        PyErr_SetString(PyExc_ValueError, "Failed to convert Python object to PXCF value.");
        return NULL;
    }
    
    char* out_str = NULL;
    PxcfError err;
    bool ok = pxcf_serialize_string(val, &out_str, &err);
    pxcf_value_free(val);
    
    if (!ok) {
        PyErr_Format(PyExc_RuntimeError, "Failed to serialize: %s", err.message);
        return NULL;
    }
    
    PyObject* result = PyUnicode_FromString(out_str);
    free(out_str);
    return result;
}

static PyMethodDef PxcfMethods[] = {
    {"parse_string", parse_string, METH_VARARGS, "Parse a PXCF string."},
    {"dump_string", dump_string, METH_VARARGS, "Serialize a dictionary to a PXCF string."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef pxcfmodule = {
    PyModuleDef_HEAD_INIT,
    "pxcf.core",
    "PXCF C11 Parser Core",
    -1,
    PxcfMethods
};

PyMODINIT_FUNC PyInit_core(void) {
    return PyModule_Create(&pxcfmodule);
}
