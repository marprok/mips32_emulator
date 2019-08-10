#include "mips32.h"
#include <string.h>

int main(void)
{
	word * w = (word*)inst_mem;
	*w = 0x10050001;
	reg_file[5] = 0x0;
	reg_file[15] = 0xFF;
	w = (word*)(inst_mem + 12);
	*w = 0x1000FFFD;
	emulate();
	return 0;
}
