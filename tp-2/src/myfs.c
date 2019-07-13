#include <stdlib.h>
#include <stdio.h>

#include "inode.h"
#include "myfs.h"
#include "vfs.h"

FSInfo *fsInfo;

//Funcao para verificacao se o sistema de arquivos está ocioso, ou seja,
//se nao ha quisquer descritores de arquivos em uso atualmente. Retorna
//um positivo se ocioso ou, caso contrario, 0.
int isIdleFn(Disk *d)
{
  for (int i = 1; i <= MAX_FDS; i++)
    if (inodeLoad(i, d) != NULL)
      return 0;

  return 1;
}

int installMyFS()
{
  fsInfo = malloc(sizeof(FSInfo));

  fsInfo->fsname = "DTFS";
  fsInfo->isidleFn = isIdleFn;
  fsInfo->fsid = (char) vfsRegisterFS(fsInfo);
}
