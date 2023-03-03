		AREA	|.text|, CODE, READONLY, ALIGN=2
		THUMB

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; System Call Table
HEAP_TOP	EQU		0x20001000
HEAP_BOT	EQU		0x20004FE0
MAX_SIZE	EQU		0x00004000		; 16KB = 2^14
MIN_SIZE	EQU		0x00000020		; 32B  = 2^5
	
MCB_TOP		EQU		0x20006800      ; 2^10B = 1K Space
MCB_BOT		EQU		0x20006BFE
MCB_ENT_SZ	EQU		0x00000002		; 2B per entry
MCB_TOTAL	EQU		512				; 2^9 = 512 entries
	
INVALID		EQU		-1				; an invalid id
	
;
; Each MCB Entry
; FEDCBA9876543210
; 00SSSSSSSSS0000U					S bits are used for Heap size, U=1 Used U=0 Not Used

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Memory Control Block Initialization
; void _kinit( )
; this routine must be called from Reset_Handler in startup_TM4C129.s
; before you invoke main( ) in driver_keil
		EXPORT	_kinit
_kinit
        ; you must correctly set the value of each MCB block
        ; complete your code
        ;PUSH    {R1-R12, LR}
        ldr        R4, =MCB_TOP
        ldr        R5, =MAX_SIZE
        MOV        R8,    #0x00
        STR        R5, [R4],#4        ;store max_size into address mcb_top
        LDR        R1,    =0x20005000    ;END POINT    LOOP 1
        LDR        R2,    =0x20001000 ;START POINT LOOP 1
        LDR        R6,    =0x20006C00    ;END POINT    LOOP 2
       
		
KNLOOP  CMP        R2, R1
        BEQ        KNLOOP2
        STR        R8, [R2],#4
        ADD        R2,  R2,#4
        B		   KNLOOP
        
        
KNLOOP2    CMP        R4, r6
        BEQ        _KNEND
        STR        R8, [R4],#4
       
        B        KNLOOP2
        
_KNEND    
        BX        lr


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Kernel Memory Allocation
; void* _k_alloc( int size )
		EXPORT	_kalloc
_kalloc
		; complete your code
		; return value should be saved into r0
		PUSH		{LR}
		LDR R1,	 	=MCB_TOP	;LEFT
		LDR R2, 	=MCB_BOT	;RIGHT
 		LDR	R3,		=MCB_ENT_SZ	; 2B per entry
		BL		_ralloc
		POP		{LR}
		BX		lr
		
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;HELPER FUNCTION FOR KALLOC



_ralloc
;_ralloc(size,left_mcb_addr,right_mcb_addr)
;R0=SIZE,R1=LEFT,R2=RIGHT,R3=ENT_SIZE

        PUSH     {LR}
        SUB        R4,R2,R1        ;LEFT MCB- RIGHT MCB//TEMP
        ADD        R5,R3,R4        ;ENTIRE MCB
        LSR        R4,R5,#1        ;half_mcb_addr_space LSR BY 1 ONE RESULTS IN DIVISION BY 2
        ADD        R7,R1,R4        ;MIDPOINT OF MCB
        MOV        R8,#0            ;initializes heap address to 0
        LSL        R9,R5,#4        ;act_entire_heap_size   LSL BY 4 RESULTS IN X16
        LSL        R10,R4,#4        ;act_half_heap_size     LSL BY 4 RESULTS IN X16
        CMP        R0,R10            ;CHECK SIZE VS act_half_heap_size
        BGT        _ELSE

;size <= act_half_heap_size)
_IF
        
        PUSH    {R0-R12}
        LDR        R3,=MCB_ENT_SZ
        SUB        R2,R7,R3        ;midpoint_mcb_addr - mcb_ent_sz PARM 3 FOR RECURSIVE CALL
        ;PUSH        {R1-R12}
        BL        _ralloc            ;recursive call with updated parm
        POP        {R1-R12}
        
        MOV        R8,R0            ;UPDATED HEAP ADDRESS
        POP        {R0}            ;GET SIZE
        CMP        R8,#0            ;CHECK IF heap_addr == 0
        BEQ        _IF_HEAP_0
        LDR        R11,[R7]        ;MIDPOINT LOADED TO CHECK IF ODD OR EVEN
        LSRS     R11, #1         ; Shift the bits in R11 to the right by 1 bit
        STRHCC     R10, [R7]       ; WRITE THE VALUE OF act_half_heap_size TO MIDPOINT
        POP        {LR}
        MOV        R0,R8            ;RETURN HEAP ADDRESS
        BX        LR
        
;heap_addr == 0
_IF_HEAP_0
        MOV        R1,R7            ;SET PARM 2 AS THE MIDPOINT
        BL        _ralloc            ;RECURSIVE CALL WITH NEW PARM
        POP        {LR}
        BX        LR

;size > act_half_heap_size
_ELSE
        LDR        R11, [R1]         ; Load the value at the memory location specified by R6 + R7 (i.e. the value at array[m2a(left_mcb_addr)]) into R12
        AND        R12, R11, #1      ; AND R12 with 1 to check if the last bit is not 0
        BEQ     return_0        ; If the last bit is 0, jump to the return_0 label
_ELSE1                            ; If the last bit is not 0, continue with the rest of the code
        CMP        R12,R9            ;
        BLT skip_return       ; If the value in R11 is less than the value in R19, skip the return instruction
        MOV     R0, #0          ; Set the value of R0 to 0
        POP        {LR}
        BX         LR
       
        
skip_return
        
        ORR        R11,R9,#0x01            ;ACT_ENTIRE_HEAP|0x01
        STRH    R11,[R1]        ;STR THE VALUE AT R11 INTO R1
        LDR        R11,=MCB_TOP    ;ORIGNAL VALUE OF MCB TOP
        SUB        R1,R1,R11        ;LEFT-MCBTOP
        LSL        R1,#4            ;X16
        LDR        R11,=HEAP_TOP    ;ORIGNAL HEAP VALUE
        ADD        R0,R11,R1        ;STORED IN R0 FOR RETURN
        POP        {LR}
        BX        LR


return_0
        MOV     R0, #0          ; Set the value of R0 to 0
        POP        {LR}
        BX         LR                ; Return from the function and restore the value of lr
		
		


	
		
		
		
		
		
		
		
		
		
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Kernel Memory De-allocation
; void *_kfree( void *ptr )	
		EXPORT	_kfree
_kfree
		; complete your code
		; return value should be saved into r0
		BX		lr
		
		END