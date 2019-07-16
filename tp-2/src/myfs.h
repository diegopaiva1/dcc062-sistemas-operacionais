/*
*  myfs.h - Funcoes que permitem a instalcao e uso de um novo sistema de arquivos
*
*  Autor: Diego Paiva e Silva & Thaynara Ferreira de Souza
*  Projeto: Trabalho Pratico II - Sistemas Operacionais
*  Organizacao: Universidade Federal de Juiz de Fora
*  Departamento: Dep. Ciencia da Computacao
*
*
*/

#ifndef MYFS_H
#define MYFS_H

#include "disk.h"

typedef struct File {
  Disk *disk;
  const char *path;
  unsigned int fd;
  LinkDir link;
} File;

typedef struct LinkDir
{
    char filename[100];
    unsigned int inumber;
} LinkDir;

int isIdleFn(Disk *d);
int openFn (Disk *d, const char *path);
int installMyFS(void); // Funcao para instalar um novo sistema de arquivos no S.O.

#endif
