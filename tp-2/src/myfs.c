#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "inode.h"
#include "myfs.h"
#include "vfs.h"

FSInfo *fsInfo;
File *files[MAX_FDS] = {NULL};

int isIdleFn(Disk *d)
{
  for (int i = 0; i < MAX_FDS; i++)
    if (files[i] != NULL)
      return 0;

  return 1;
}

File* __getFile(Disk* d, const char* path)
{
  for (int i = 0; i < MAX_FDS; i++)
    if (files[i] != NULL && files[i]->disk == d && strcmp(files[i]->path, path) == 0)
      return files[i];

  return NULL;
}

int openFn(Disk* d, const char* path)
{
  File *file = __getFile(d, path);

  // Arquivo não existe, então vamos criá-lo
  if (file == NULL) {
    Inode *inode = NULL;

    for (int i = 0; i < MAX_FDS; i++) {
      if (files[i] == NULL) {
        inode = inodeCreate(i + 1, d);
        break;
      }
    }

    // Memória insuficiente ou number inválido
    if (inode == NULL)
      return -1;

    file = malloc(sizeof(File));
    file->disk = d;
    file->path = path;
    file->fd = inodeGetNumber(inode);
    files[file->fd - 1] = file;
  }

  return file->fd;
}

int readFn(int fd, char *buf, unsigned int nbytes)
{
  Inode *inode = NULL;

  for (int i = 0; i < MAX_FDS; i++)
    if (files[i] != NULL && files[i]->fd == fd)
      inode = inodeLoad(fd, files[i]->disk);

  if (inode == NULL)
    return -1;

}

int installMyFS()
{
  fsInfo = malloc(sizeof(FSInfo));

  fsInfo->fsname = "DTFS";
  fsInfo->isidleFn = isIdleFn;
  fsInfo->openFn = openFn;
  fsInfo->fsid = (char) vfsRegisterFS(fsInfo);
}
