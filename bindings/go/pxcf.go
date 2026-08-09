// Package pxcf provides bindings to the native C11 PXCF parser ecosystem.
package pxcf

/*
#cgo CFLAGS: -I../../include
#cgo LDFLAGS: -L../../build -lpxcf
#include <stdlib.h>
#include "pxcf/pxcf.h"
*/
import "C"
import (
	"errors"
	"fmt"
	"unsafe"
)

// Document represents a parsed PXCF document.
type Document struct {
	ptr *C.PxcfDocument
}

// Parse parses a PXCF string into an abstract document.
func Parse(source string) (*Document, error) {
	cSource := C.CString(source)
	defer C.free(unsafe.Pointer(cSource))
	
	var err C.PxcfError
	docPtr := C.pxcf_parse_string(cSource, C.size_t(len(source)), &err)
	
	if docPtr == nil {
		return nil, fmt.Errorf("Parse error %d: %s", int(err.code), C.GoString(&err.message[0]))
	}
	
	return &Document{ptr: docPtr}, nil
}

// Free releases the C memory held by the document.
func (d *Document) Free() {
	if d.ptr != nil {
		C.pxcf_document_free(d.ptr)
		d.ptr = nil
	}
}

// Stringify serializes the document back to a string.
func (d *Document) Stringify() (string, error) {
	if d.ptr == nil {
		return "", errors.New("document is nil or freed")
	}
	
	root := C.pxcf_document_root(d.ptr)
	if root == nil {
		return "", errors.New("document has no root")
	}
	
	var err C.PxcfError
	var outStr *C.char
	
	ok := C.pxcf_serialize_string(root, &outStr, &err)
	if !ok {
		return "", fmt.Errorf("Serialize error %d: %s", int(err.code), C.GoString(&err.message[0]))
	}
	
	defer C.free(unsafe.Pointer(outStr))
	return C.GoString(outStr), nil
}

// Validate checks if a PXCF string is syntactically valid.
func Validate(source string) bool {
	doc, err := Parse(source)
	if err != nil {
		return false
	}
	doc.Free()
	return true
}
