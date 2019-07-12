#include <stdlib.h>
#include <stdio.h>

#include "myfs.h"
#include "vfs.h"

FSInfo *fsInfo;

int installMyFS()
{
  fsInfo = malloc(sizeof(FSInfo));

  fsInfo->fsname = "DTFS";
  fsInfo->fsid = (char) vfsRegisterFS(fsInfo);
}


