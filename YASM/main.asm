	global _main
	extern _printf
	extern _system

	section .text

_main:
	mov eax,01H
	add eax,02H
	push message
	call _printf
	add esp,4
	push cmdline
	call _system
	mov eax,0
	ret

message:
	db  'YASM Welcome Message.',10,0
cmdline:
	db  'pause',0