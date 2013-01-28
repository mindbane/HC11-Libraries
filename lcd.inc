********************************
* Function: LCDDEC
* draw a binary fraction to the screen
* param 1: dd ram starting address
* param 2: 1 byte binary fraction to print
* param 3: 1 byte decimal place count to output
********************************
LCDDEC	STS		STACKP
		PSHX
		PSHY
		PSHB
		PSHA
		LDX		STACKP
		PSHX
		
		LDY		#$00
		LDAB	$05,X
		ABY
		LDAB	$04,X
		LDAA	$03,X
		STAA	RETURN
		LDX		#$00
		
LOOPDEC	LDAA	#!10
		MUL
		ORAA	#$30
		PSHA
		LDAA	RETURN
		PSHA
		JSR		LCDCHAR
		INCA
		STAA	RETURN
		CMPB	#$00
		BEQ		LCDDECEND
		DEY
		BNE		LOOPDEC
		
LCDDECEND
		PULY
		LDX		$01,Y
		STX		$04,Y
		PULA
		PULB
		PULY
		PULX
		INS
		INS
		INS
		RTS
*********************************************
* Function: LCDBCD
* draws a bcd digit to the screen
* param 1: dd ram starting address
* param 2: byte value
*********************************************
LCDBCD	STS		STACKP	; STORE STACK POINTER TEMPORARILY
		PSHX			; SAVE VARIABLES
		PSHY			;
		PSHB			;
		PSHA

		LDX		STACKP
		PSHX

		LDAA	$03,X		; PUT ADDRESS INTO A
		LDAB	$04,X		; BYTE IS NOW IN B

		LSRB
		LSRB
		LSRB
		LSRB
		PSHB
		JSR		BCDTOA
		LDAB	RETURN
		PSHB
		PSHA
		JSR		LCDCHAR
		INCA

		LDAB	$04,X
		ANDB	#$0F
		PSHB
		JSR		BCDTOA
		LDAB	RETURN
		PSHB
		PSHA
		JSR		LCDCHAR
		INCA
		STAA	RETURN

		PULY
		LDX		$01,Y
		STX		$03,Y
		PULA
		PULB				; RESTORE VARIABLES
		PULY
		PULX
		INS
		INS
		RTS

****************************************************
* Function: LCDBIN8
* draws a single byte to the lcd display, in binary
* param1: 1 byte integer
****************************************************
LCDBIN8	STS		STACKP	; STORE STACK POINTER TEMPORARILY
		PSHX			; SAVE VARIABLES
		PSHY			;
		PSHB			;
		PSHA

		LDX		STACKP
		PSHX
		LDAB	$03,X
		LDAA	#!15

LOOP	ANDB	#$01
		ORAB	#$30
		PSHB
		LDAB	$03,X
		LSRB
		STAB	$03,X
		PSHA
		JSR		LCDCHAR
		DECA
		CMPA	#$8
		BGE		LOOP
		LDAA	#!16
		STAA	RETURN
		PULY
		LDX		$01,Y
		STX		$02,Y
		PULA
		PULB				; RESTORE VARIABLES
		PULY
		PULX
		INS
		RTS

****************************************************
* Function: LCDCHAR
* draws a single character to the LCD display
* param1: 1 byte dd ram address
* param2: 1 byte character
****************************************************
LCDCHAR	STS		STACKP	; STORE STACK POINTER TEMPORARILY
		PSHX			; SAVE VARIABLES
		PSHY			;
		PSHB			;
		PSHA

		LDX		STACKP
		PSHX

		LDAA	$03,X	; PUT ADDRESS INTO A
;rs = b0
;rw = b1
		; to write a char, set dram address by setting rs,rw to 0, and putting the address with an msb of 1 into portc
		LDY		#PORTA	;
		LDAB	#$FC
		STAB	PORTC
		ORAA	#$80
		STAA	PORTB
		BSET	$00,Y,$40
		BCLR	$00,Y,$40
		; delay
		LDY		#$1
		PSHY
		LDY		#$1
		PSHY
		JSR		DELAY		;Delay for LCD instructions
		; now actually write the character
		LDAA	$04,X		; character is now in A
		LDY		#PORTA		;
		LDAB	#$FD		;
		STAB	PORTC		;
		STAA	PORTB		;
		BSET	$00,Y,$40	; SET ENABLE
		BCLR	$00,Y,$40	; CLEAR ENABLE
		; delay
		LDY		#$1
		PSHY
		LDY		#$1
		PSHY
		JSR		DELAY		;Delay for LCD instructions

		PULY
		LDX		$01,Y
		STX		$03,Y
		PULA
		PULB				; RESTORE VARIABLES
		PULY
		PULX
		INS
		INS
		RTS
********************************
* INITLCD
* Initializes the LCD Display
********************************
*PortA = E (1 line -> PA6)
*PortB = LCD RS, R/W (2 lines, PB0,1)
*PortC = LCD Data Bus (8 lines)
INITLCD	PSHX
		PSHY
		PSHB
		PSHA

		LDX		#PORTA		;
		LDAB	#$FC		;
		STAB	PORTC		;
		LDAA	#%00111100	;Function Set: 001DL NFxx
		STAA	PORTB		; DL: 1
							; N: 1
							; F: 1
		BSET	$00,X,$40	;
		BCLR	$00,X,$40	;
							; DELAY FOR 43uS = 86 instructions
							; THIS IS GREATER THAN THE MINIMUM DELAY OF OUR DELAY CODE
							; WHICH IS 116 instructions or 56uS
		LDY		#$1
		PSHY
		LDY		#$1
		PSHY
		JSR		DELAY		;Delay for LCD instructions

		LDAA	#$01		;Display Clear
		STAA	PORTB
		BSET	$00,X,$40	;
		BCLR	$00,X,$40	;
		LDY		#$1
		PSHY
		LDY		#!530
		PSHY
		JSR		DELAY		;Delay for 1.64ms
		LDAA	#%00001100	;Display ON/OFF Control: 0000 1DCB
		STAA	PORTB		; D: 1
							; C: 0
							; B: 0
		BSET	$00,X,$40	;
		BCLR	$00,X,$40	;
		LDY		#$1			; DELAY AGAIN
		PSHY
		LDY		#$1
		PSHY
		JSR		DELAY		;Delay for LCD instructions
		LDAA	#%00000110	;Entry Mode Set: 0000 01I/DS
		STAA	PORTB		; I/D: 1
							; S: 0
		BSET	$00,X,$40	;
		BCLR	$00,X,$40	;
		LDY		#$1			; DELAY AGAIN
		PSHY
		LDY		#$1
		PSHY
		JSR		DELAY
		PULA
		PULB
		PULY
		PULX
		RTS

********************************
* LCDCLR
* Clears the LCD Display
********************************
LCDCLR	PSHA
		PSHB
		PSHX
		PSHY

		LDX		#PORTA
		LDAB	#$FC
		STAB	PORTC
		LDAA	#%00000001
		STAA	PORTB
		BSET	$00,X,$40
		BCLR	$00,X,$40
		LDY		#$1
		PSHY
		LDY		#!530
		PSHY
		JSR		DELAY		;Delay for 1.64ms

		PULY
		PULX
		PULB
		PULA
		RTS

*****************************************
* HEX2C
* Converts a hex nibble to a single byte
*****************************************
HEX2C	STS		STACKP
		PSHX
		PSHY
		PSHB
		PSHA
		LDY		STACKP
		PSHY

		LDAA	$03,Y
		CMPA	#!10
		BLO		DIGIT
		ADDA	#$37
		BRA		FINISHH
DIGIT	ADDA	#$30
FINISHH	STAA	RETURNL
		PULY
		LDX		$01,Y
		STX		$02,Y
		PULA
		PULB			; RESTORE VARIABLES
		PULY
		PULX
		INS
		RTS
************************************
* LCDHEX1B
* OUTPUTS THE HEX VALUE OF A 1 BYTE HEX VALUE
* Param 1: dd ram address
* param 2: 8 bit number
************************************
LCDHEX1B STS	STACKP
		PSHX
		PSHY
		PSHB
		PSHA
		
		LDY		STACKP
		PSHY
		
		LDAB	$03,Y
		LDAA	$04,Y
		LSRA
		LSRA
		LSRA
		LSRA
		PSHA
		JSR		HEX2C
		LDAA	RETURNL
		PSHA
		PSHB
		JSR		LCDCHAR

		LDAA	$04,Y
		ANDA	#$0F
		PSHA
		JSR		HEX2C
		LDAA	RETURNL
		PSHA
		INCB
		PSHB
		JSR		LCDCHAR

		INCB
		STAB	RETURN
		PULY
		LDX		$01,Y
		STX		$03,Y
		PULA
		PULB				; RESTORE VARIABLES
		PULY
		PULX
		INS
		INS
		RTS

************************************
* LCDUI16
* OUTPUTS AN UNSIGNED 16 BIT VALUE TO THE LCD SCREEN
* Param 1: dd ram address
* Param 2: 16 bit number
************************************
LCDUI16	STS		STACKP	; STORE STACK POINTER TEMPORARILY
		PSHX			; SAVE VARIABLES
		PSHY			;
		PSHB			;
		PSHA

		LDY		STACKP
		PSHY

		; LOAD A WITH THE DD RAM ADDRESS
		LDAA	$03,Y
		; OUTPUT THE 10000'S PLACE, IF NEEDED
		LDX		#!10000
		PSHX
		LDX		$04,Y
		PSHX
		JSR		GETDIG
		LDAB	RETURNL
		BEQ		THOUS
		ORAB	#$30
		PSHB
		PSHA
		JSR		LCDCHAR
		INCA	; THE NEW DD RAM ADDRESS

THOUS	LDX		#!1000
		PSHX
		LDX		$04,Y
		PSHX
		JSR		GETDIG
		LDAB	RETURNL
		CMPA	$03,Y	; HAVE WE OUTPUT ANY CHARACTERS YET?
		BNE		OUTTHOU
		CMPB	#$00	; WE HAVE NOT OUTPUT ANY CHARACTERS, BUT SHOULD WE OUTPUT THIS ONE?
		BEQ		HUNDR	; NO
OUTTHOU	ORAB	#$30
		PSHB
		PSHA
		JSR		LCDCHAR
		INCA

HUNDR	LDX		#!100
		PSHX
		LDX		$04,Y
		PSHX
		JSR		GETDIG
		LDAB	RETURNL
		CMPA	$03,Y	; HAVE WE OUTPUT ANY CHARACTERS YET?
		BNE		OUTHUND
		CMPB	#$00	; WE HAVE NOT OUTPUT ANY CHARACTERS, BUT SHOULD WE OUTPUT THIS ONE?
		BEQ		TENS	; NO
OUTHUND	ORAB	#$30
		PSHB
		PSHA
		JSR		LCDCHAR
		INCA

TENS	LDX		#!10
		PSHX
		LDX		$04,Y
		PSHX
		JSR		GETDIG
		LDAB	RETURNL
		CMPA	$03,Y	; HAVE WE OUTPUT ANY CHARACTERS YET?
		BNE		OUTTENS
		CMPB	#$00	; WE HAVE NOT OUTPUT ANY CHARACTERS, BUT SHOULD WE OUTPUT THIS ONE?
		BEQ		ONES	; NO
OUTTENS	ORAB	#$30
		PSHB
		PSHA
		JSR		LCDCHAR
		INCA

ONES	LDX		#!1
		PSHX
		LDX		$04,Y
		PSHX
		JSR		GETDIG
		LDAB	RETURNL
		; WE OUTPUT THE ONES DIGIT NO MATTER WHAT
		ORAB	#$30
		PSHB
		PSHA
		JSR		LCDCHAR
		INCA

		STAA	RETURN
		PULY
		LDX		$01,Y
		STX		$04,Y


		PULA
		PULB				; RESTORE VARIABLES
		PULY
		PULX
		INS
		INS
		INS
		RTS

****************************************************
* Function: LCDSTR
* draws a string character to the LCD display
* param1: 1 byte dd ram starting address
* param2: 2 byte string address
****************************************************
LCDSTR	STS		STACKP	; STORE STACK POINTER TEMPORARILY
		PSHX			; SAVE VARIABLES
		PSHY			;
		PSHB			;
		PSHA

		LDX		STACKP
		PSHX
		LDAB	#!0
		LDY		$04,X
STRLOOP	LDAA	$00,Y
		BEQ		END
		PSHA

		LDAA	$03,X
		ABA
		PSHA

		JSR		LCDCHAR

		INCB
		INY
		BRA		STRLOOP

END		ADDB	$03,X
		STAB	RETURN
		PULY
		LDX		$01,Y
		STX		$04,Y
		PULA
		PULB				; RESTORE VARIABLES
		PULY
		PULX
		INS
		INS
		INS
		RTS

******************************************
* Converts a BCD digit to an ascii digit
******************************************
BCDTOA	STS		STACKP	; STORE STACK POINTER TEMPORARILY
		PSHX			; SAVE VARIABLES
		PSHY			;
		PSHB			;
		PSHA

		LDX		STACKP
		PSHX

		LDAA	$03,X		; PUT ADDRESS INTO A
		CMPA	#!10
		BGE		LETTER
		ORAA	#$30
		BRA		RET

LETTER	ADDA	#$31

RET		STAA	RETURN
		PULY
		LDX		$01,Y
		STX		$02,Y
		PULA
		PULB				; RESTORE VARIABLES
		PULY
		PULX
		INS
		RTS
