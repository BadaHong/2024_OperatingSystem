#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"

//system call for getting grand parent process id
int
getgpid(void)
{
	return myproc()->parent->parent->pid;
};

//Wrapper for getgpid()
int
sys_getgpid(void)
{
	return getgpid();
};
