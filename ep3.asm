* OUR STUFF
STACKP	EQU		$0000	; USING 0000-0001
RETURN	EQU		$0002
RETURNL	EQU		$0003
CORRECT	EQU		$0004
SWAP	EQU		$0005

*START OF PROGRAM
		ORG		$D000
START	LDS		#$1FF
		LDAA	#$FF
		STAA	DDRC
		JSR		INITLCD

		LDAA	#$38
		STAA	DDRD
		LDAB	#$0A

		LDAA	#$80
		STAA	OPTION

LOOP1	LDAA	#$00
		STAA	ADCTL

* AD DONE NOT ADD ONE
ADDONE	LDAA	ADCTL
		ANDA	#$80
		BEQ		ADDONE

		LDX		#ADRES
		PSHX
		LDAA	#$00
		PSHA
		JSR		LCDSTR

		LDAA	ADR1
		PSHA
		LDAA	RETURN
		PSHA
		JSR		LCDHEX1B

		LDX		#VOLTS
		PSHX
		LDAA	#$40
		PSHA
		JSR		LCDSTR

		LDAA	ADR1
		LDAB	#!05
		MUL
		LDX		#!255
		IDIV

		PSHB
		PSHA
		PSHX
		LDAA	RETURN
		PSHA
		JSR		LCDUI16

		LDX		#DECIMAL
		PSHX
		LDAA	RETURN
		PSHA
		JSR		LCDSTR

		PULA
		PULB
		LDX		#!255
		FDIV

		LDAA	#$2
		PSHA

		PSHX
		PULA
		PULB
		PSHA

		LDAA	RETURN
		PSHA

		JSR		LCDDEC

		LDX		#V
		PSHX
		LDAA	RETURN
		PSHA
		JSR		LCDSTR

		LDAA	ADR1
		LDAB	#$00
		LDX		#!25
		IDIV
		PSHX
		PULA
		PULB
		PSHA
		JSR		BAROUT

		LDX		#!50000
		PSHX
		LDX		#!1
		PSHX
		JSR		DELAY


		JSR		LCDCLR
		JMP		LOOP1

***********************
* BARCLR
* Sets the blank bit
***********************
BARCLR	PSHX
		LDX		#PORTC
		BSET	$00,X,$40
		NOP
		BCLR	$00,X,$40
		PULX
		RTS

********************
* BAROUT
* Outputs some number of bars
********************
BAROUT	STS		STACKP
		PSHX
		PSHY
		PSHB
		PSHA

		LDX		STACKP
		PSHX

		JSR		BARCLR

		LDAB	$03,X
		LDY		#$00
		ABY
		LDD		#$00
BLOOP	CPY		#$00
		BEQ		BEND
		LSLD
		ORAB	#$01
		DEY
		BRA		BLOOP
BEND	PSHA
		JSR		SMOUT
		PSHB
		JSR		SMOUT

		LDX		#PORTC
		BSET	$00,X,$80
		NOP
		BCLR	$00,X,$80

		PULY
		LDX		$01,Y
		STX		$02,Y
		PULA
		PULB
		PULY
		PULX
		INS
		RTS

*****************************************
* SMOUT                                 *
* Outputs a single byte over the serial *
* connection.                           *
*****************************************
SMOUT	STS		STACKP	; STORE STACK POINTER TEMPORARILY
		PSHX			; SAVE VARIABLES
		PSHY			;
		PSHB			;
		PSHA

		LDX		STACKP
		PSHX

		LDAA	$03,X		; PUT ADDRESS INTO A

		STAA	SPDR
		LDAA	#$50
		STAA	SPCR

POLLSPR	LDAA	SPSR
		ANDA	#$80
		BEQ		POLLSPR

		PULY
		LDX		$01,Y
		STX		$02,Y
		PULA
		PULB				; RESTORE VARIABLES
		PULY
		PULX
		INS
		RTS



******************************************************
* Includes go down here to deal with quirky debugger *
******************************************************
#INCLUDE "ports.inc"
#INCLUDE "lcd.inc"
#INCLUDE "math.inc"
#INCLUDE "timing.inc"

********************************************************
* Predefined strings to send to sent to the LCD display
********************************************************
ADRES	FCB		'Result: ',$00
VOLTS	FCB		'Volts: ',$00
V		FCB		'V',$00			; 22 CHARACTERS TOTAL
DECIMAL	FCB		'.',$00

		ORG		$FFFE
		FDB		START
