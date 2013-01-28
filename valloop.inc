******************************************************
* VALLOOP
* Loops over a value and checks for BCD
******************************************************
VALLOOP	STS		STACKP	; STORE STACK POINTER TEMPORARILY
		PSHX			; SAVE VARIABLES
		PSHY			;
		PSHB			;
		PSHA

		LDX		STACKP
		PSHX
		LDAB	$03,X
		LDX		#$8
		LDAA	#$01
LOOP8	JSR		LCDCLR
		PSHB
		JSR		DOVAL
		LDY		#!30000		; DELAY FOR ABOUT A SECOND
		PSHY
		LDY		#!10
		PSHY
		JSR		DELAY
		PSHA
		PSHB
		JSR		ROTATER
		LDAB	RETURN
		DEX
		BNE		LOOP8

		PULY
		LDX		$01,Y
		STX		$02,Y
		PULA
		PULB				; RESTORE VARIABLES
		PULY
		PULX
		INS
		RTS

*********************************************
* Checks the value for BCD, then outputs it
*********************************************
DOVAL	STS		STACKP	; STORE STACK POINTER TEMPORARILY
		PSHX			; SAVE VARIABLES
		PSHY			;
		PSHB			;
		PSHA

		LDX		STACKP
		PSHX
		LDAB	$03,X

		PSHB
		JSR		LCDBIN8

		PSHB
		JSR		VALBCD
		LDAA	RETURN

		BEQ		VALID
		LDX		#BADBCD
		PSHX
		LDAA	#$47
		PSHA
		JSR		LCDSTR
		BRA		FINISHV
VALID	LDX		#GOODBCD
		PSHX
		LDAA	#$45
		PSHA
		JSR		LCDSTR
		LDAA	RETURN
		PSHB
		PSHA
		JSR		LCDBCD
FINISHV	PULY
		LDX		$01,Y
		STX		$02,Y
		PULA
		PULB				; RESTORE VARIABLES
		PULY
		PULX
		INS
		RTS
