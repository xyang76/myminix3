#include <sys/cdefs.h>
#include <lib.h>
#include "namespace.h"

#include <unistd.h>

#ifdef __weak_alias
__weak_alias(unlink, _unlink)
#endif

int undelete(name)
const char *name;
{
  message m;

  _loadname(name, &m);
  return(_syscall(VFS_PROC_NR, UNLINK, &m));
}

int rcmkdir(const char *name, mode_t mode)
{
  message m;

  m.m1_i1 = strlen(name) + 1;
  m.m1_i2 = mode;
  m.m1_p1 = (char *) __UNCONST(name);
  return(_syscall(VFS_PROC_NR, MKDIR, &m));
}
