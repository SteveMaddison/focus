#include "focus/interrupt.h"
#include "focus/util.h"
#include "focus/errno.h"
#include "focus/types.h"

struct intr_task_s *task_start;

/* Don't put code before this function -
 * it is alligned precisely at 0x0038!
 */
void intr_handle( void )
{
    struct intr_task_s *task = task_start;
    intr_disable();
    while( task )
    {
        if( task->ticks++ == task->interval ) {
            task->ticks = 0;
            (*task->function)();
        }
        task = task->next;
    }
    intr_enable();
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

    printf("registered intr_task %x\n", task );
    printf("funcation @ %x\n", task->function );

    return 0;
}
