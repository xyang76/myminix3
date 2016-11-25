#include <sys/cdefs.h>
#include <lib.h>
#include "namespace.h"

#include <unistd.h>

#ifdef __weak_alias
__weak_alias(unlink, _unlink)
#endif

int fundelete(file)
const char *file;
{
    struct stat sb;
    message m;
    
    printf("in fundelete\n");
//    
//    _DIAGASSERT(file != NULL);
//
//    if (lstat(file, &sb) < 0)
//        return -1;
//
//    if (S_ISDIR(sb.st_mode))
//        m.m3_i2 = 1;
//    else
//        m.m3_i2 = 2;
//    _loadname(name, &m);
//    return(_syscall(VFS_PROC_NR, UNDELETE, &m));
    return 0;
}

int rcmkdir(const char *name, mode_t mode)
{
  message m;

  m.m1_i1 = strlen(name) + 1;
  m.m1_i2 = mode;
  m.m1_p1 = (char *) __UNCONST(name);
  return(_syscall(VFS_PROC_NR, RCMKDIR, &m));
}
