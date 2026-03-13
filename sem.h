#ifndef _SEM_H_
#define _SEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct semun
{
    int val;
};

void UI_sem_init();

unsigned int UI_sem_p();

void UI_sem_v();

void UI_sem_destroy();


#ifdef __cplusplus
}
#endif

#endif

