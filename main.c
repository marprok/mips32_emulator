#include "mips32.h"
#include <string.h>

int main(void)
{
	memset(inst_mem, 0x00, sizeof(inst_mem));
	word * w = (word*)inst_mem;
	*w = 0x00AF802A;
	reg_file[5] = 0x10;
	reg_file[15] = 0xFF;
	emulate();
	return 0;
}
