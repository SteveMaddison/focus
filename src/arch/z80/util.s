;
;  File         : arch_Z80.s
;  Description  : Architecture-dependent assembly source for the Z80
;
;  Begin        : Mon Nov 12 2001
;  Copyright    : (C) 2001 by Steve Maddison
;  Email        : steve@cosam.org
;
;
;  This program is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; either version 2 of the License, or
;  (at your option) any later version.
;


;-------------------------------
;  global function identifiers
;-------------------------------

; prototypes declared in 'util.h'
	.globl _outb			; i/o port output
	.globl _inb				; i/o port input

; prototypes declared 'util.h'
	.globl _get_sp			; get stack pointer


;-----------------
;  function code
;-----------------
	.area _CODE

;----------------------------------------------------
;  void outb( BYTE port, BYTE byte );
;
;  outputs <byte> to i/o port <port>
;----------------------------------------------------
_outb::
	push	bc
	ld		hl, #0x0004
	add		hl, sp
	ld		c, (hl)
	inc		hl
	ld		b, (hl)
	out		(c), b
	pop		bc
	ret


;----------------------------------------------------
; BYTE inb( BYTE port );
;
; returns byte on i/o port <port>
;----------------------------------------------------
_inb::
	push	bc
	ld		hl, #0x0004
	add		hl, sp
	ld		c,(hl)
	in		l,(c)
	ld		h, #0x00
	pop		bc
	ret


;----------------------------------------------------
; BYTE inbfe( BYTE port );
;
; same as above, but with B register set to 0xfe
; to enable high address lines
;----------------------------------------------------
_inb_fe::
	push	bc
	ld		hl, #0x0004
	add		hl, sp
	ld		b,(hl)
	ld		c, #0xfe
	in		l,(c)
	ld		h, #0x00
	pop		bc
	ret


;----------------------------------------------------
; size_t* get_sp( void );
;
; returns the stack pointer
;----------------------------------------------------
_get_sp::
	ld		hl, #0x0000
	add		hl, sp
	ret


;----------------------------------------------------
; void reset( void );
;
; soft reset
;----------------------------------------------------
_reset::
	jp		0
