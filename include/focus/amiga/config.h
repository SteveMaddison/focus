#ifndef _AMIGA_H_
#define _AMIGA_H_

#define MACHINE_NAME		"Commodore Amiga"
#define CPU_Z80

/*
 * 256Kb ROM at 0xf00000
 * 256Kb ROM at 0xfc0000
 */
#define ROM_START		0xf00000
#define ROM_SIZE		0x80000
#define ROM_END			((ROM_START)+(ROM_SIZE)-1)

/*
 * RAM starts at 0x000000
 */
#define RAM_START		0x000000
#define RAM_SIZE		0x80000   /* 512Kb */
#define RAM_END			((RAM_START)+(RAM_SIZE)-1)

/*
 * 8Kb VRAM located at begining of physical RAM
 */
#define VRAM_START		0x000000
#define VRAM_SIZE		0x14000
#define VRAM_END		((VRAM_START)+(VRAM_SIZE)-1)

/*
 * Kernel stack of 2Kb
 */
#define KSTACK_SIZE		2048
#define KSTACK_END		0x07ffff
#define KSTACK_START		((KSTACK_END)-(KSTACK_SIZE)+1)

/*
 * Block sizes for memory management
 */
#define MEM_BLOCK_LARGEST	4096	/* Largest block size */
#define MEM_BLOCK_SMALLEST	64	/* Smallest block size */
#define MEM_BLOCK_BLOCKS	5	/* Number of blocks of each size */

#endif /* _AMIGA_H_ */

