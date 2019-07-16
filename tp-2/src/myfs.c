#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "inode.h"
#include "myfs.h"
#include "vfs.h"

FSInfo *fsInfo;
File *files[MAX_FDS] = {NULL};

//Funcao para verificacao se o sistema de arquivos está ocioso, ou seja,
//se nao ha quisquer descritores de arquivos em uso atualmente. Retorna
//um positivo se ocioso ou, caso contrario, 0.
int isIdleFn(Disk *d)
{
  for (int i = 0; i < MAX_FDS; i++)
    if (files[i] != NULL)
      return 0;

  return 1;
}

File* __getFile(Disk *d, const char *path)
{
  for (int i = 0; i < MAX_FDS; i++)
    if (files[i] != NULL)
      if (files[i]->disk == d && strcmp(files[i]->path, path) == 0)
        return files[i];

  return NULL;
}

int openFn(Disk *d, const char *path)
{
  File *file = __getFile(d, path);

  // Arquivo não existe, então vamos criá-lo
  if (file == NULL) {
    Inode *inode = NULL;

    for (int i = 1; i <= MAX_FDS; i++) {
      if (inodeLoad(i, d) == NULL) {
        inode = inodeCreate(i, d);
        break;
      }
    }

    // Memória insuficiente ou number inválido
    if (inode == NULL)
      return -1;

    file = malloc(sizeof(File));
    file->disk  = d;
    file->path  = path;
    file->fd    = inodeGetNumber(inode);
    //file->link = NULL;
    files[file->fd - 1] = file;
  }

  return file->fd;
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

int installMyFS()
{
  fsInfo = malloc(sizeof(FSInfo));

  fsInfo->fsname = "DTFS";
  fsInfo->isidleFn = isIdleFn;
  fsInfo->openFn = openFn;
  fsInfo->fsid = (char) vfsRegisterFS(fsInfo);
}
