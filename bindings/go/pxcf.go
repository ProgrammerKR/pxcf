// Package pxcf provides bindings to the native C11 PXCF parser ecosystem.
package pxcf

/*
#cgo CFLAGS: -I../../include
#cgo LDFLAGS: -L../../build -lpxcf
#include "pxcf/pxcf.h"
*/
import "C"
import "errors"

// Parse parses a PXCF string into an abstract document.
func Parse(source string) (interface{}, error) {
    return nil, errors.New("not yet implemented")
}
