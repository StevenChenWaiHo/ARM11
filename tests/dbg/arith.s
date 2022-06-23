start:
; set r1 = 4
mov r1, #4
; ret r2 = 5
mov r2, #5
; set r3 += r1 + r2
add r3, r1, r2
add r5, r5, r3

cmp r5, #100
blt start 

mov r4, #66
