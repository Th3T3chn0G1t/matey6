[org 0x8000]
[bits 16]
[cpu 8086]

__reset:

add cx, cx
sbb cx, cx
add cx, [0x8000]
add cx, [bx]
add [0x8000], cx
add [bx], cx

hlt

times 0x7FFE - ($ - $$) nop
dw __reset
