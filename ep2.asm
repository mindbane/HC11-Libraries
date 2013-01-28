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

		JSR		BARCLR

		LDAA	#$0A
		PSHA
		JSR		BAROUT

PBHIGH	LDAA	PORTA	; Wait for pushbutton to be pressed and released
		ANDA	#$01
		BNE		PBHIGH

PBLOW	LDAA	PORTA
		ANDA	#$01
		BEQ		PBLOW

		LDX		#!5000
		PSHX
		LDX		#!1
		PSHX
		JSR		DELAY

		DECB
		CMPB	#$FF
		BNE		OUTB
		LDAB	#$0A
OUTB	PSHB
		JSR		BAROUT
		BRA		PBHIGH

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

