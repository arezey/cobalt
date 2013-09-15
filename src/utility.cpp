#include <ctime>
#include "main.h"

// =============================================================================
// Checks a string against a mask
// -----------------------------------------------------------------------------
bool mask (CoString string, CoString mask) {
	// Elevate to uppercase for case-insensitive matching
	string = +string;
	mask = +mask;
	
	const char* chars = string.chars();
	const char* maskstring = mask.chars();
	const char* mptr = &maskstring[0];
	
	for (const char* sptr = chars; *sptr != '\0'; sptr++) {
		if (*mptr == '?') {
			if (*(sptr + 1) == '\0') {
				// ? demands that there's a character here and there wasn't.
				// Therefore, mask matching fails
				return false;
			}
		} elif (*mptr == '*') {
			char end = *(++mptr);
			
			// If '*' is the final character of the message, all of the remaining
			// string matches against the '*'. We don't need to bother checking
			// the string any further.
			if (end == '\0')
				return true;
			
			// Skip to the end character
			while (*sptr != end && *sptr != '\0')
				sptr++;
			
			// String ended while the mask still had stuff
			if (*sptr == '\0')
				return false;
		} elif (*sptr != *mptr)
			return false;
		
		mptr++;
	}
	
	return true;
}

str join (const CoStringList& args, str delim) {
	str rep = "";
	
	for (const str& i : args) {
		if (rep.length() > 0)
			rep += delim;
		
		rep += i;
	}
	
	return rep;
}

int posof (const str& a, int n) {
	int count = 0;
	
	for (int i = 0; i < a.length(); ++i) {
		if (a[i] != ' ')
			continue;
		
		if (++count < n)
			continue;
		
		return i;
	}
	
	return -1;
}