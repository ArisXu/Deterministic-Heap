#include "dheap.h"


STATUS activateLFHBucket(size_t size) {
	for (size_t i = 0; i < 0x20 + ((unsigned int)rand()%10); ++i) {
		if (!malloc(size)) {
			return FAIL;
		}
	}
	return SUCCESS;
}

STATUS activeAll() 
{
	for (size_t i = 0;;) 
	{
		if (activateLFHBucket(i + 1) == FAIL) return FAIL;
		if (i < 0x100) i += 8;
		else if (i < 0x400) i += 0x10;
		else if (i < 0x800) i += 0x40;
		else if (i < 0x1000) i += 0x80;
		else if (i < 0x2000) i += 0x100;
		else if (i < 0x3e00) i += 0x200;
		else break;
	}
	return SUCCESS;
}


void init() 
{
	cout << "Loading......" << endl;
	srand((unsigned int)__rdtsc());
	//HANDLE hHeap = HeapCreate(0x0, 0x0, 0x0);
	if (activeAll() == FAIL)exit(-1);
}

FixedBufGrow<FixedBufBase*> root_array(10, "Root Node");

int main() 
{
	init();
	cout << "Deterministic Heap v1.0" << endl;
	while (1) root_array.select();
}