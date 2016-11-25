#ifndef __MFS_IDELETE_H__
#define __MFS_IDELETE_H__

struct idelete {
  ino_t i_dir;			/* which directory is the inode on */
  ino_t i_num;			/* inode number on its (minor) device */
  u16_t i_mode;			/* file type, protection, etc. */
  char i_name[MFS_DIRSIZ]; /* inode name */
};

#endif
