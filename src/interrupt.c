#include "config.h"
#ifdef ARCH_SPECTRUM
#endif
#include "focus/interrupt.h"
#include "focus/memory.h"
#include "focus/util.h"


void intr_init( void )
{
    /* set up the interrupt table */
    static void *ptr;

    printf( "ptr @ %x\n", ptr );

/*
    intr_enable();
*/
}

