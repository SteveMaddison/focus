#ifndef _SPECTRUM_H_
#define _SPECTRUM_H_

#define MACHINE_NAME		"ZX Spectrum"
#define ARCH_SPECTURM
#define CPU_Z80

/*
 * 16Kb ROM at 0x0000
 */
#define ROM_START		0x0000
#define ROM_SIZE		0x4000
#define ROM_END			((ROM_START)+(ROM_SIZE)-1)

/*
 * RAM starts at 0x4000
 * config.h: RAM_SIZE
 */
#ifndef RAM_SIZE
# define RAM_SIZE		0xc000	/* 48Kb */
#endif
#define RAM_START		0x4000
#define RAM_END			((RAM_START)+(RAM_SIZE)-1)

/*
 * 6Kb Video RAM, plus 768 bytes for attributes
 * Located at begining of physical RAM
 */
#define VRAM_START		0x4000
#define VRAM_SIZE		0x1b00
#define VRAM_END		((VRAM_START)+(VRAM_SIZE)-1)
#define ARAM_START		0x5800	/* attribute table */
#define THIRD_SIZE		2048
#define ROW_SIZE		256

/*
 * Kernel stack of 512b
 * config.h: KSTACK_SIZE
 */
#ifndef KSTACK_SIZE
# define KSTACK_SIZE		512
#endif
#define KSTACK_END		0xffff
#define KSTACK_START		((KSTACK_END)-(KSTACK_SIZE)+1)

/*
 * Block sizes for memory management
 */
#define MEM_BLOCK_LARGEST	4096	/* Largest block size */
#define MEM_BLOCK_SMALLEST	64	/* Smallest block size */
#define MEM_BLOCK_BLOCKS	5	/* Number of blocks of each size */


#endif /* _SPECTRUM_H_ */

