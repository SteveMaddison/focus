;
;  File         : interrupt.s
;  Description  : Architecture-dependent interrupt stuff for Specturm
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

; prototypes declared 'spectrum/interrupt.h'
	.globl _intr_enable		; enable interrupts
	.globl _intr_disable	; disable interrupts
	.globl _intr_handle		; link to C code (interrupt.c)

;----------------------------------------------------------------------
; If all goes well, the code here should end up at 0x0038, and will be
; entered when an interrupt fires. All we do is save our context and
; call the C handler function with interrupts disabled.
;----------------------------------------------------------------------
_int_catch::
	di						; disable interrupts
	push	af				; store registers
	push	bc
	push	de
	push	hl

	call	_intr_handle	; call C handler

	pop		hl				; restore registers
	pop		de
	pop		bc
	pop		af
	ei						; re-enable interrupts
	reti					; return from interrupt handler


;----------------------------------------------------
; void intr_enable( void );
;
; switches on interrupts
;----------------------------------------------------
_intr_enable::
	ei
	ret


;----------------------------------------------------
; void intr_disable( void );
;
; switches off interrupts
;----------------------------------------------------
_intr_disable::
	di
	ret
