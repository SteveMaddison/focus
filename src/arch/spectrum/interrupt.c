#include "focus/interrupt.h"
#include "focus/util.h"
#include "focus/errno.h"
#include "focus/types.h"

/* Head of a linked list containing tasks which are triggered
 * by interrupts.
 */
struct intr_task_s *task_start;


/* This function is called from intr.s with interrupts disabled
 * whenever an interrupt is fired. On the Spectrum, this is once
 * every 50 seconds.
 * Each task has an "interval" parameter which specifies after
 * how many ticks (==interrupts fired) the task's function
 * should be executed.
 */
void intr_handle( void )
{
    struct intr_task_s *task = task_start;

    while( task )
    {
        if( task->ticks++ == task->interval ) {
        	/* number of ticks has elapsed - run the function! */
            (*task->function)();
            task->ticks = 0;
        }
        task = task->next;
    }
}

void intr_init( void )
{
    task_start = NULL;
}

/* Register an interrupt task.
 */
int intr_task_register( struct intr_task_s *task )
{
    task->next  = task_start;
    task->ticks = 0;
    task_start  = task;

    return 0;
}
