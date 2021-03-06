#ifndef __WAD_H__
#define __WAD_H__

#include "common.h"
#include "types.h"

// NOTE: In the WAD file, the lump name is only 8 characters. The 9th character
// is always a null terminator and is added manually after the structure is read.
// There are no conflicts with this and reading the WAD since the label is the
// last element in the structure.
typedef struct {
	long offset;
	long size;
	char label[9];
} lumpinfo;

class WAD {
	str wadname;
	bool iwad;
	long numlumps;
	vector<lumpinfo> directory;
	byte* data;
	ulong datalen;
	
public:
	WAD (const char* filepath);
	~WAD ();
	
	bool init;
	str errormsg;
};

#endif // __WAD_H__