[org 0x8000]
[bits 16]
[cpu 8086]

__reset:
sbb cx, cx
add cx, cx
add cx, [0x8000]
add cx, [bx]
add [0x8000], cx
add [bx], cx

times 0x7FFD - ($ - $$) nop
dw __reset + 2 ; reset vector
