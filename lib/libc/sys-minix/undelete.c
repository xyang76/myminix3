#include <sys/cdefs.h>
#include <lib.h>
#include "namespace.h"

#include <unistd.h>

#ifdef __weak_alias
__weak_alias(unlink, _unlink)
#endif

int fundelete(const char *file)
{
    message m;

    _loadname(name, &m);
    return(_syscall(VFS_PROC_NR, FUNDELETE, &m));
}

int rcmkdir(const char *name, mode_t mode)
{
    message m;

    m.m1_i1 = strlen(name) + 1;
    m.m1_i2 = mode;
    m.m1_p1 = (char *) __UNCONST(name);
    return(_syscall(VFS_PROC_NR, RCMKDIR, &m));
}
