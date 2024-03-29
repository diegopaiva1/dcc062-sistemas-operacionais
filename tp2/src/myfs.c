#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "inode.h"
#include "myfs.h"
#include "vfs.h"

FSInfo *fsInfo;
File *files[MAX_FDS] = {NULL};

int installMyFS()
{
  fsInfo = malloc(sizeof(FSInfo));

  fsInfo->fsname = "DTFS";
  fsInfo->fsid = (char) vfsRegisterFS(fsInfo);
  fsInfo->isidleFn = isIdleFn;
  //fsInfo->formatFn = formatFn;
  fsInfo->openFn = openFn;
  fsInfo->readFn = readFn;
  //fsInfo->writeFn = writenFn;
  //fsInfo->closeFn = closeFn;
  //fsInfo->opendirFn = opendirFn;
  //fsInfo->readdirFn = readdirFn;
  fsInfo->linkFn = linkFn;
  //fsInfo->unlinkFn = unlinkFn;
  //fsInfo->closedirFn = closedirFn;

}

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
    //file->link = NULL;
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

int linkFn(int fd, const char *filename, unsigned int inumber) // TODO conferir se ja existe entrada de nome filename
{
    if(fd < 0 || fd >= MAX_FDS) return -1;
    File* dir = files[fd];
    if(dir == NULL) 
      return -1;
    Inode* inode = inodeLoad(inumber, dir->disk);
    if(inode == NULL)
      return -1;

    LinkDir link;
    strcpy(link.filename, filename);
    link.inumber = inumber;
    dir->link = link;

    return 0;
}
