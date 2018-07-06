#ifndef __OBJECTCODE_H_
#define __OBJECTCODE_H_

typedef struct REG REG;

struct REG{
	int regno;
	int useornot;
};

void objectcode(Intercodes *head,FILE *f);

#endif
