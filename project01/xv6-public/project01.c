#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
        printf(1, "My student id is 2022074057\n");
        int pid = getpid();
        int gpid = getgpid();
        printf(1, "My pid is %d\nMy gpid is %d\n", pid, gpid);
	exit();
};

