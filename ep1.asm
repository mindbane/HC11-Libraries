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


		LDX		#!50000
		PSHX
		LDX		#$01
		PSHX
		JSR		DELAY


		JSR		LCDCLR
		BRA 	LOOP1





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
V		FCB		'V',$00
DECIMAL	FCB		'.',$00

		ORG		$FFFE
		FDB		START
