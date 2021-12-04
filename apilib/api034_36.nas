[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api028.nas"]

		GLOBAL		_api_get_mx
		GLOBAL		_api_get_my
		GLOBAL		_api_get_mouse_data

[SECTION .text]

_api_get_mx:		; int api_get_mx(void);
		MOV		EDX,34
		INT		0x40
		RET

_api_get_my:		; int api_get_my(void);
		MOV		EDX,35
		INT		0x40
		RET
