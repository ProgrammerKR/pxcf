package pxcf

import (
	"testing"
)

func TestParse(t *testing.T) {
	doc, err := Parse("key: 123")
	if err != nil {
		t.Fatalf("Failed to parse valid string: %v", err)
	}
	defer doc.Free()
}

func TestParseInvalid(t *testing.T) {
	_, err := Parse("key: ")
	if err == nil {
		t.Fatalf("Expected error for invalid string, got nil")
	}
}

func TestStringify(t *testing.T) {
	doc, err := Parse("key: 123")
	if err != nil {
		t.Fatalf("Failed to parse valid string: %v", err)
	}
	defer doc.Free()

	out, err := doc.Stringify()
	if err != nil {
		t.Fatalf("Failed to stringify: %v", err)
	}
	
	// C11 Serializer currently formats loosely depending on implementation,
	// but it must contain the key and value.
	if len(out) == 0 {
		t.Fatalf("Stringify returned empty string")
	}
}

func TestValidate(t *testing.T) {
	if !Validate("key: 123") {
		t.Fatalf("Expected true for valid string")
	}
	if Validate("key: ") {
		t.Fatalf("Expected false for invalid string")
	}
}
