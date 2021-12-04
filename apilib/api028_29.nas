[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api028.nas"]

		GLOBAL	_api_read_serial
		GLOBAL	_api_write_serial

[SECTION .text]

_api_read_serial:		; char api_read_serial(void);
		MOV		EDX,28
		INT		0x40
		RET

_api_write_serial:		; void api_write_serial(char);
		MOV		EDX,29
		MOV		EBX,[ESP+8]
		INT		0x40
		RET

