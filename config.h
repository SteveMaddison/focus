#ifndef _CONFIG_H_

/* define the architecture, which should define CPU type */
#define ARCH_SPECTRUM

/* total RAM */
#define RAM_SIZE		0xc000	/* 48Kb */

/* size (bytes) of kernel stack */
#define KSTACK_SIZE		512

/* done last to ensure our options are passed on */
#ifdef ARCH_SPECTRUM
# include "focus/spectrum/config.h"
#endif

#endif /* _CONFIG_H_ */

