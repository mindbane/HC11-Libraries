*************************
* Function: DELAY
* Delays for 94 + (16 + (6 * PARAMETER 2)) * PARAMETER 1 cycles
*************************
DELAY	STS		STACKP	;								; 4 CYCLES
		PSHX			; SAVE VARIABLES				; 4 CYCLES
		PSHY			; 								; 5 CYCLES
		PSHB			;								; 3 CYCLES
		PSHA			;								; 3 CYCLES

		LDX		STACKP	;								; 4 CYCLES
		PSHX			;								; 4 CYCLES

		LDY		$03,X	;								; 6 CYCLES
		LDD		$05,X	;								; 5 CYCLES

LOOPY	PSHB			;								; 3 CYCLES
		PSHA			;								; 3 CYCLES
		PULX			;								; 5 CYCLES

LOOPX	DEX				; COUNT DOWN X					; 3 CYCLES
		BNE		LOOPX	; UNTIL X IS 0					; 3 CYCLES

		DEY				; COUNT DOWN Y					; 4 CYCLES
		BNE		LOOPY	; UNTIL Y IS 0					; 3 CYCLES


		PULY			;								; 6 CYCLES
		LDX		$01,Y	;								; 6 CYCLES
		STX		$05,Y	;								; 6 CYCLES
		PULA											; 4 CYCLES
		PULB			; RESTORE VARIABLES				; 4 CYCLES
		PULY											; 6 CYCLES
		PULX											; 5 CYCLES
		INS				;								; 3 CYCLES
		INS				;								; 3 CYCLES
		INS				;								; 3 CYCLES
		INS				;								; 3 CYCLES
		RTS				; RETURN FROM SUB				; 5 CYCLES

********************
* WAITOV
* WAIT FOR OVERFLOW
* RETURNS ORIGINAL TCNT
********************
WAITOV	PSHA
		LDAA	TCNT	; 3 CYCLES
		STAA	RETURN	; 3 CYCLES

WAITER	LDAA	TCNT	; 3 CYCLES
		CMPA	#!11	; 2 CYCLES
		BLO		WAITER	; 3 CYCLES
		PULA
		RTS

**********************
* WAIT1K
* WAIT FOR .5ms = 1000 cycles
**********************
WAIT1K 	PSHA
		PSHB
		PSHY

		LDAA	#$04
		STAA	TCTL1

		; SET TOC4 TO TCNT SO THAT WE CAN SIMPLY ADD PARAMETERS TO TOC4
		LDD		TCNT
		ADDD	#!1000
		STD		TOC4


		; PERFORM INSTRUCTION WHEN OC4 IS ENABLED
		LDAA	#$10
		STAA	TFLG1

POLLL	BITA	TFLG1
		BEQ		POLLL

		PULY
		PULB
		PULA
		RTS

****************************************************************
* TIMEUSR
* waits to see how long it takes for the user to press a button
****************************************************************
TIMEUSR	PSHB
		LDAB	TCNT
		PSHX			; SAVE VARIABLES
		PSHY			;
		PSHA

		; ENABLE INPUT COMPARE 1 TO CAPTURE ON RISING EDGE
		LDAA	#$10
		STAA	TCTL2

		; ENABLE READING FROM TIC1
		LDAA	#$04
		STAA	TFLG1
		LDX		#$00
TIMER	JSR		WAIT1K	; .5ms
		INX
		BITA	TFLG1
		BEQ		TIMER

		; We waited for X * .5 ms on average.

		STX		RETURN

		PULA
		PULY
		PULX
		PULB				; RESTORE VARIABLES
		RTS
*****************************************
* SLEEP1S
* Despite the name, sleeps for 2 seconds
*****************************************
SLEEP1S	PSHY
		LDY		#!60
		PSHY
		LDY		#!10000
		PSHY
		JSR		DELAY
		PULY
		RTS
