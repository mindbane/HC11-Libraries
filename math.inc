*******************************
* AVGU10
* Accepts 10 2 byte parameters
*******************************
AVGU10	STS		STACKP
		PSHX
		PSHY
		PSHB
		PSHA

		LDX		STACKP
		PSHX

		LDY		STACKP

		LDD		$03,Y
		LDX		#!10
		IDIV
		PSHX

		ADDD	$05,Y
		LDX		#!10
		IDIV
		PSHX

		ADDD	$07,Y
		LDX		#!10
		IDIV
		PSHX

		ADDD	$09,Y
		LDX		#!10
		IDIV
		PSHX

		ADDD	$0B,Y
		LDX		#!10
		IDIV
		PSHX

		ADDD	$0D,Y
		LDX		#!10
		IDIV
		PSHX

		ADDD	$0F,Y
		LDX		#!10
		IDIV
		PSHX

		ADDD	$11,Y
		LDX		#!10
		IDIV
		PSHX

		ADDD	$13,Y
		LDX		#!10
		IDIV
		PSHX

		ADDD	$15,Y
		LDX		#!10
		IDIV
		PSHX

		LDD		#$00

		PULX
		STX		RETURN
		ADDD	RETURN

		PULX
		STX		RETURN
		ADDD	RETURN

		PULX
		STX		RETURN
		ADDD	RETURN

		PULX
		STX		RETURN
		ADDD	RETURN

		PULX
		STX		RETURN
		ADDD	RETURN

		PULX
		STX		RETURN
		ADDD	RETURN

		PULX
		STX		RETURN
		ADDD	RETURN

		PULX
		STX		RETURN
		ADDD	RETURN

		PULX
		STX		RETURN
		ADDD	RETURN

		PULX
		STX		RETURN
		ADDD	RETURN

		STD		RETURN

		PULY
		LDX		$01,Y
		STX		$15,Y
		PULA
		PULB
		PULY
		PULX
		INS
		INS
		INS
		INS
		INS
		INS
		INS
		INS
		INS
		INS
		INS
		INS
		INS
		INS
		INS
		INS
		INS
		INS
		INS
		INS
		RTS


**********************
* Function: ROTATER
* Rotates byte to the right 'n' times
* Param 1: Byte to rotate
* Param 2: n
**********************
ROTATER	STS		STACKP	; STORE STACK POINTER TEMPORARILY
		PSHX			; SAVE VARIABLES
		PSHY			;
		PSHB			;
		PSHA

		LDX		STACKP
		PSHX
		LDAB	$03,X
		LDAA	$04,X
ROTLOOP	CLC
		RORB
		BCC		CARRYC
		ORAB	#$80
CARRYC	DECA
		BNE		ROTLOOP
DONE	STAB	RETURN
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

***************************************************
* GETDIG
* GET THE N'TH DIGIT (BASE 10) FROM A 16 BIT UNSIGNED NUMBER
* Param 1: 16 BIT VALUE
* Param 2: WHICH PLACE DO YOU WANT (2 bytes) (EG: 1, 10, 100, 1000, ...)
***************************************************
GETDIG	STS		STACKP
		PSHX
		PSHY
		PSHB
		PSHA
		LDY		STACKP
		PSHY

		LDD		$03,Y

		LDX		#!10000
		IDIV
		STX		RETURN
		LDX		$05,Y
		CPX		#!10000
		BEQ		ENDDIG

		LDX		#!1000
		IDIV
		STX		RETURN
		LDX		$05,Y
		CPX		#!1000
		BEQ		ENDDIG

		LDX		#!100
		IDIV
		STX		RETURN
		LDX		$05,Y
		CPX		#!100
		BEQ		ENDDIG

		LDX		#!10
		IDIV
		STX		RETURN
		LDX		$05,Y
		CPX		#!10
		BEQ		ENDDIG

		LDX		#!1
		IDIV
		STX		RETURN

ENDDIG	PULY
		LDX		$01,Y
		STX		$05,Y
		PULA
		PULB				; RESTORE VARIABLES
		PULY
		PULX
		INS
		INS
		INS
		INS
		RTS

*************************
* Function: VALBCD
* Tests to see if first parameter is a valid BCD number
* Returns a 00 if its valid, a FF if invalid
*************************
VALBCD 	STS		STACKP	; STORE STACK POINTER TEMPORARILY
		PSHX			; SAVE VARIABLES
		PSHY			;
		PSHB			;
		PSHA

		LDX		STACKP
		PSHX

		LDAB	$03,X

		ANDB	#$0F
		CMPB	#$09
		BHI		NONBCD
		LDAB	$03,X
		ANDB	#$F0
		CMPB	#$90
		BHI		NONBCD
		LDAB	$03,X
		LDAA	#$00
		STAA	RETURN

ENDBCD	PULY
		LDX		$01,Y
		STX		$02,Y
		PULA
		PULB			; RESTORE VARIABLES
		PULY
		PULX
		INS
		RTS
NONBCD	LDAA	#$FF
		STAA	RETURN
		BRA		ENDBCD

******************************
* SHUFFLE
* Param 1: address of array (2 byte elements)
* param 2: length (1 byte)
******************************
*Fisher-Yates shuffle
*To shuffle an array a of n elements:
*  for i from n - 1 downto 1 do
*       j ? random integer with 0 ? j ? i
*       exchange a[j] and a[i]
******************************
SHUFFLE	STS		STACKP
		PSHA
		PSHB
		PSHX
		PSHY
		LDX		STACKP
		PSHX

		LDAB	$05,X
		DECB
		; B = i
		; A ?
SLOOP	STAB	RETURNL
		LDX		#$00
		ABX
		LDD		TCNT
		IDIV
		LDY		STACKP
		LDX		$03,Y
		LSLB
		ABX
		PSHX
		LDY		$00,X
		PSHY
		LDAB	RETURNL
		LDY		STACKP
		LDX		$03,Y
		LSLB
		ABX
		PSHX
		LDY		$00,X
		PSHY
		STS		SWAP
		LDX		SWAP
		LDY		$03,X
		STY		SWAP
		LDY		$07,X
		STY		$03,X
		LDY		SWAP
		STY		$07,X
		PULY
		PULX
		STY		$00,X
		PULY
		PULX
		STY		$00,X
		LDAB	RETURNL
		DECB
		CMPB	#$01
		BHI		SLOOP

		PULY
		LDX		$01,Y
		STX		$04,Y
		PULY
		PULX
		PULB
		PULA
		INS
		INS
		INS
		RTS
