/* TLPI, Ch. 3, Exercise 1 */
#include <stdio.h>
#include <unistd.h>
#include <linux/reboot.h>

int main(void)
{
	printf("LINUX_REBOOT_MAGIC1  = 0x%08x  (%d)\n", LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC1);
	printf("LINUX_REBOOT_MAGIC2  = 0x%08x  (%d)\n", LINUX_REBOOT_MAGIC2, LINUX_REBOOT_MAGIC2);
	printf("LINUX_REBOOT_MAGIC2A = 0x%08x  (%d)\n", LINUX_REBOOT_MAGIC2A, LINUX_REBOOT_MAGIC2A);
	printf("LINUX_REBOOT_MAGIC2B = 0x%08x  (%d)\n", LINUX_REBOOT_MAGIC2B, LINUX_REBOOT_MAGIC2B);
	printf("LINUX_REBOOT_MAGIC2C = 0x%08x  (%d)\n", LINUX_REBOOT_MAGIC2C, LINUX_REBOOT_MAGIC2C);
		
	return 0;
}
	
