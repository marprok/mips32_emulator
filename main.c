#include "mips32.h"
#include <string.h>

int main(void)
{
	memset(inst_mem, 0x00, sizeof(inst_mem));
	word * w = (word*)inst_mem;
	*w = 0x00AF8020;
	reg_file[5] = 5;
	reg_file[15] = -2;
	emulate();
	return 0;
}
