#ifndef SHARED_H
#define SHARED_H

#include <stdint.h>

#define MARK_TEST			0xE3546EEAC4102B2Dull
#define MARK_ANOTHER		0x0123456789abcdefull

#pragma pack(push,1)
struct PackedBuffer {
	uint64_t marker;     // Marker for detecting the value
	char buffer[1024];   // Data you need
	// Can add additional checks, like checksum
};
#pragma pack(pop)


extern const PackedBuffer value_test;		// test value
extern const PackedBuffer value_another;	// anither value for testing. because one is not enough for me

#endif