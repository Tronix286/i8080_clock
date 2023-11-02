
	module clock80_crt0


;--------
; Include zcc_opt.def to find out some info
;--------

        defc    crt0 = 1
        INCLUDE "zcc_opt.def"

;--------
; Some scope definitions
;--------

        EXTERN    _main           ;main() is always external to crt0 code
        EXTERN    asm_im1_handler

        PUBLIC    cleanup         ;jp'd to by exit()
        PUBLIC    l_dcal          ;jp(hl)


        defc    TAR__clib_exit_stack_size = 0
        defc    TAR__register_sp = 0xFE00	;-1
	defc    TAR__fputc_cons_generic = 0
	defc	CLIB_DISABLE_FGETS_CURSOR = 1
	defc	CRT_KEY_DEL = 127
	defc	__CPU_CLOCK = 3555555
;	defc	__CPU_CLOCK = 1777000
	
	; RAM trimming
	defc	DEFINED_CLIB_FOPEN_MAX = 1
	defc	CLIB_FOPEN_MAX = 3


        defc CRT_ORG_CODE = 0x0000
        defc    CRT_ORG_DATA = 0x8000
        defc    CRT_ORG_BSS  = 0x8000

        ;defc    CLIB_EXIT_STACK_SIZE = 0


        defc    TAR__crt_enable_rst = $0000

        INCLUDE "crt/classic/crt_rules.inc"
        defc    CONSOLE_COLUMNS = 64
        defc    CONSOLE_ROWS = 25

	PUBLIC	GRAPHICS_CHAR_SET
	PUBLIC	GRAPHICS_CHAR_UNSET
	defc	GRAPHICS_CHAR_SET = 127
	defc	GRAPHICS_CHAR_UNSET = 32


	org	  CRT_ORG_CODE

IF CRT_ORG_CODE = 0x0000

if (ASMPC<>$0000)
        defs    CODE_ALIGNMENT_ERROR
endif

	di
	jp	program
db "(c)Tronix "
;	INCLUDE	"crt/classic/crt_z80_rsts.asm"
ENDIF

program:
        INCLUDE "crt/classic/crt_init_sp.asm"
        INCLUDE "crt/classic/crt_init_atexit.asm"
	call    crt0_init_bss
	ld	hl,0
	add	hl,sp
	ld	(exitsp),hl
    	;ei
; Optional definition for auto MALLOC init
; it assumes we have free space between the end of
; the compiled program and the stack pointer
IF DEFINED_USING_amalloc
    INCLUDE "crt/classic/crt_init_amalloc.asm"
ENDIF
	ld	hl,0
	push	hl	;argv
	push	hl	;argc
	call	_main
cleanup:
	jp	$f800

l_dcal: jp      (hl)            ;Used for function pointer calls



; If we were given a model then use it
IF DEFINED_CRT_MODEL
    defc    __crt_model = CRT_MODEL
ELSE
    defc    __crt_model = 2 ; zx7
ENDIF
        defc    __crt_org_bss = CRT_ORG_BSS

	INCLUDE "crt/classic/crt_runtime_selection.asm" 
	
	INCLUDE	"crt/classic/crt_section.asm"
;-------------------------------------------------------------------------
; Native terminal i/o functionality for micro8085_cilb (stdio interface)

        SECTION code_clib

        EXTERN  getk

        PUBLIC  fgetc_cons
        PUBLIC  _fgetc_cons

;-------------------------------------------------------------------------
; Function will stall until data is available to pull from buffer
fgetc_cons:
_fgetc_cons:
        ret

;-------------------------------------------------------------------------
; Native terminal i/o functionality for micro8085_cilb (stdio interface)
; Functions below assumes support for INTEL 8085 "undocumented" opcodes

        SECTION code_clib

        EXTERN  puartd
        EXTERN  puartc

        PUBLIC  fputc_cons_native
        PUBLIC  _fputc_cons_native

;-------------------------------------------------------------------------
; Prints char and inserts CR ahead of LF, i.e. printf("\n") becomes "\r\n"
fputc_cons_native:
_fputc_cons_native:
        ret

IF ( __crt_model = 2)
	SECTION bss_clib

	PUBLIC zaloha 

	zaloha:
  	defb   0

        SECTION code_clib
	SECTION code_compress_zx7

	PUBLIC asm_dzx7_standard
	EXTERN zaloha

asm_dzx7_standard:

;-----------------------------------------------------------------------------
; ZX7 decoder by Einar Saukas
; "Standard" version
; i8080 port Roman Borik (108 bytov)
;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------
; Parameters:
;   HL: source address (compressed data)
;   DE: destination address (decompressing)
;------------------------------------------------------------------------------
dzx7:           mvi     a,80h
                sta     zaloha
copy_byte_loop: mov     a,m                 ; copy literal byte
                stax    d
                inx     h
                inx     d
main_loop:      call    next_bit
                jnc     copy_byte_loop      ; next bit indicates either literal or sequence

; determine number of bits used for length (Elias gamma coding)
                push    d                   ; destination onto stack
                lxi     b,0                 ; zero length
                mov     d,b
length_size_loop:
                inr     d
                call    next_bit
                jnc     length_size_loop

; determine length
length_value_loop:
                cnc     next_bit
                mov     a,c
                ral
                mov     c,a
                mov     a,b
                ral
                mov     b,a
                jc      exit                ; check end marker
                dcr     d
                jnz     length_value_loop
                inx     b                   ; adjust length

; determine offset
                mov     a,m                 ; load offset flag (1 bit) + offset value (7 bits)
                inx     h
                stc
                ral
                mov     e,a
                jnc     offset_end          ; if offset flag is set, load 4 extra bits
                mvi     d,0E0h              ; bit marker to load 4 bits
rld_next_bit:   call    next_bit
                mov     a,d                 ; insert next bit into D
                ral
                mov     d,a
                jc      rld_next_bit        ; repeat 4 times, until bit marker is out
                inr     d                   ; add 128 to DE
                mov     a,d                 ; retrieve fourth bit from D
                rar
                mov     d,a
offset_end:     mov     a,e                 ; insert fourth bit into E
                rar
                mov     e,a

; copy previous sequence
                xthl                        ; store source, restore destination
                push    h                   ; store destination
                mov     a,l
                sbb     e
                mov     l,a
                mov     a,h
                sbb     d
                mov     h,a                 ; HL = destination - offset - 1
                pop     d                   ; DE = destination, BC = length
copy_loop:      mov     a,m
                stax    d
                inx     h
                inx     d
                dcx     b
                mov     a,b
                ora     c
                jnz     copy_loop
exit:           pop     h                   ; restore source address (compressed data)
                jnc     main_loop
                ret

next_bit:       lda zaloha	;mvi     a,0
                add     a                   ; check next bit
                sta     zaloha
                rnz                         ; no more bits left?
                mov     a,m                 ; load another group of 8 bits
                inx     h
                ral
                sta     zaloha
                ret

ENDIF