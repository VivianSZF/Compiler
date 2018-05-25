#include "hash.h"

uint32_t hash_pjw(char* name)
{
	uint32_t val = 0, i;
	for(; *name;++name)
	{
		val=(val<<2)+*name;
		if(i=val&~SIZE) val=(val^(i>>12))&SIZE;
	}
	return val;
}
