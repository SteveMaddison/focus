#include "focus/interrupt.h"
#include "focus/util.h"
#include "focus/errno.h"
#include "focus/types.h"

struct intr_task_s *task_start;

/* This function is called from intr.s with interrupts disabled */
void intr_handle( void )
{
    struct intr_task_s *task = task_start;

    while( task )
    {
        if( task->ticks++ == task->interval ) {
            task->ticks = 0;
            (*task->function)();
        }
        task = task->next;
    }
}

void intr_init( void )
{
    task_start = NULL;
}

int intr_task_register( struct intr_task_s *task )
{
    task->next  = task_start;
    task->ticks = 0;
    task_start  = task;

    return 0;
}
