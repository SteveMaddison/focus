#ifndef _SPECTRUM_INTERRUPT_H_
#define _SPECTRUM_INTERRUPT_H_

struct intr_task_s {
    void 	(*function)(void);  /* function pointer */
    int     interval;           /* number of ticks between invokations */
    int     ticks;              /* number of ticks elapsed since last invoked */
	struct  intr_task_s *next;
};

extern void intr_init( void );
extern void intr_handle( void );
extern void intr_enable( void );
extern void intr_disable( void );
extern void intr_return( void );
extern int intr_task_register( struct intr_task_s *task );

#endif /* _SPECTRUM_INTERRUPT_H_ */
