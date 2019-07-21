/*
*  lottery.c - Implementacao do algoritmo Lottery Scheduling e sua API
*
*  Autores: Diego Paiva (201565516AC) e Thaynara Ferreira (201565254C)
*  Projeto: Trabalho Pratico I - Sistemas Operacionais
*  Organizacao: Universidade Federal de Juiz de Fora
*  Departamento: Dep. Ciencia da Computacao
*
*
*/

#include "lottery.h"
#include <stdio.h>
#include <string.h>

//Nome unico do algoritmo. Deve ter 4 caracteres.
const char lottName[]="LOTT";
int slot;

//=====Funcoes Auxiliares=====






//=====Funcoes da API=====

//Funcao chamada pela inicializacao do S.O. para a incializacao do escalonador
//conforme o algoritmo Lottery Scheduling
//Deve envolver a inicializacao de possiveis parametros gerais
//Deve envolver o registro do algoritmo junto ao escalonador
void lottInitSchedInfo() {
  SchedInfo *sched_info = malloc(sizeof(SchedInfo));

  strcpy(sched_info->name, lottName);
  sched_info->initParamsFn = lottInitSchedParams;
  sched_info->scheduleFn = lottSchedule;
  sched_info->releaseParamsFn = lottReleaseParams;

  slot = schedRegisterScheduler(sched_info);
}

//Inicializa os parametros de escalonamento de um processo p, chamada
//normalmente quando o processo e' associado ao slot de Lottery
void lottInitSchedParams(Process *p, void *params) {
	schedSetScheduler(p, params, slot);
}

//Retorna o proximo processo a obter a CPU, conforme o algortimo Lottery
Process* lottSchedule(Process *plist) {
  int total_tickets = 0;

  for (Process *p = plist; p != NULL; p = processGetNext(p))
  {
    LotterySchedParams *sched_params = processGetSchedParams(p);

    if (processGetStatus(p) != PROC_WAITING)
      total_tickets += sched_params->num_tickets;
  }

	int n = rand() % total_tickets;
  int sum = 0;

  for (Process *p = plist; p != NULL; p = processGetNext(p))
  {
    LotterySchedParams *sched_params = processGetSchedParams(p);

    if (processGetStatus(p) != PROC_WAITING) {
      sum += sched_params->num_tickets;
      if (n <= sum)
        return p;
    }
  }

	return NULL;
}

//Libera os parametros de escalonamento de um processo p, chamada
//normalmente quando o processo e' desassociado do slot de Lottery
//Retorna o numero do slot ao qual o processo estava associado
int lottReleaseParams(Process *p) {
  free(processGetSchedParams(p));
  processSetSchedParams(p, NULL);
  processSetSchedSlot(p, -1);

	return slot;
}

//Transfere certo numero de tickets do processo src para o processo dst.
//Retorna o numero de tickets efetivamente transfeirdos (pode ser menos)
int lottTransferTickets(Process *src, Process *dst, int tickets) {
	int transfered_tickets;
  LotterySchedParams *src_sched_params = processGetSchedParams(src);
  LotterySchedParams *dst_sched_params = processGetSchedParams(dst);

  if (src_sched_params->num_tickets >= tickets)
  {
    src_sched_params->num_tickets -= tickets;
    dst_sched_params->num_tickets += tickets;
    transfered_tickets = tickets;
  }
  else
  {
    dst_sched_params->num_tickets = src_sched_params->num_tickets;
    transfered_tickets = src_sched_params->num_tickets;
    src_sched_params->num_tickets = 0;
  }

  return transfered_tickets;
}
