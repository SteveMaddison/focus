;
;  File         : interrupt.s
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

; prototypes declared 'spectrum/interrupt.h'
	.globl _intr_enable		; enable interrupts
	.globl _intr_disable	; disable interrupts
	.globl _intr_return		; return from interrupt handler

;----------------------------------------------------
; void intr_enable( void );
;
; switches on interrupts
;----------------------------------------------------
_intr_enable::
    im		1
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


;----------------------------------------------------
; void intr_return( void );
;
; returns from an interrupt handler routine
; (after enabling interrupts)
;----------------------------------------------------
_intr_return::
	ei
	reti
