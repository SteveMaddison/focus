#ifndef _SPECTRUM_KEYBOARD_H_
#define _SPECTRUM_KEYBOARD_H_

extern int kb_init( void );
extern char kb_scan( void );
extern struct fifo_s* kb_buffer;

#define KB_BUFFER_SIZE      32

#define	KB_MAX_KEY			40

#define KB_SHIFT			0x01
#define KB_SYMBOL_SHIFT		0x02
#define KB_ENTER			0x06

#define KB_PGDN				0x01	/*  1d - ^A */
#define KB_LEFT				0x02	/*  2d - ^B */
#define KB_END				0x05	/*  5d - ^E */
#define KB_RIGHT			0x06	/*  6d - ^F */
#define KB_TAB				0x09	/*  9d - ^I */
#define KB_INS				0x0b	/* 11d - ^K */
#define KB_DOWN				0x0e	/* 14d - ^N */
#define KB_UP				0x10	/* 16d - ^P */
#define KB_PGUP				0x11	/* 17d - ^Q */
#define KB_HOME				0x17	/* 23d - ^W */
#define KB_ESC				0x1b	/* 27d - ^[ */
#define KB_DEL				0x7f	/*   127d  */

#endif /* _SPECTRUM_KEYBOARD_H_ */
