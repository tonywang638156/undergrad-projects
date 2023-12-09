.data #memoized DP calculation of f(x) = f(x-1) + f(x//2) + x^2
.text #f(0) = 0
.align 2
.globl main
syscall:
    beq $v0, $0, main
    addi $k1, $0, 10
    beq $v0, $k1, syscall10
    addi $k1, $0, 12
    beq $v0, $k1, syscall12
    addi $k1, $0, 80
    beq $v0, $k1, syscall80
    addi $k1, $0, 5
    beq $v0, $k1, syscall5
    addi $k1, $0, 77
    beq $v0, $k1, syscall77
    addi $k1, $0, 65
    beq $v0, $k1, syscall65

syscall10:
    j syscall10
    jr $k0

syscall12:
    lw $k1, 16($0)            #we is 0, access to 16
    beq $k1, $0, do_nothing   #
    lw $v0, 20($0)
    sw $0, 16($0)
    j finish
    do_nothing:
    addi $v0, $0, 0
    finish:
    jr $k0

syscall5:

    addi $sp, $sp, -4
    sw $t0, 0($sp)

    addi $v1, $0, 0
    read:

    addi $sp, $sp, -4
    sw $k0, 0($sp)
    addi $v0, $0, 12
    syscall
    lw $k0, 0($sp)
    addi $sp, $sp, 4

    beq $v0, $0, read
    #lw $k1, 16($0)
    #beq $k1, $0, read

    addi $t0, $0, 32
    beq $v0, $t0, stop

    beq $v1, $0, miss_mult
    addi $k1, $0, 10
    mult $v1, $k1
    mflo $v1

    miss_mult:
    #sw $0, 16($0)

    #lw $v0, 20($0)


    addi $k1, $0, 48
    bne $v0, $k1, not0
    addi $v1, $v1, 0
    j found_int
    not0:
    addi $k1, $k1, 1
    bne $v0, $k1, not1
    addi $v1, $v1, 1
    j found_int
    not1:

    addi $k1, $k1, 1
    bne $v0, $k1, not2
    addi $v1, $v1, 2
    j found_int
    not2:

    addi $k1, $k1, 1
    bne $v0, $k1, not3
    addi $v1, $v1, 3
    j found_int
    not3:

    addi $k1, $k1, 1
    bne $v0, $k1, not4
    addi $v1, $v1, 4
    j found_int
    not4:

    addi $k1, $k1, 1
    bne $v0, $k1, not5
    addi $v1, $v1, 5
    j found_int
    not5:

    addi $k1, $k1, 1
    bne $v0, $k1, not6
    addi $v1, $v1, 6
    j found_int
    not6:

    addi $k1, $k1, 1
    bne $v0, $k1, not7
    addi $v1, $v1, 7
    j found_int
    not7:

    addi $k1, $k1, 1
    bne $v0, $k1, not8
    addi $v1, $v1, 8
    j found_int
    not8:

    addi $k1, $k1, 1
    bne $v0, $k1, stop
    addi $v1, $v1, 9

    found_int:

    j read

    stop:

    addi $v0, $v1, 0

    lw $t0, 0($sp)
    addi $sp, $sp, 4

    jr $k0


syscall80:
    sw $a0, 32($0)
    sw $a1, 36($0)
    sw $a2, 40($0)
    sw $0, 44($0)
    #addi $v0, $0, 18
    jr $k0

syscall77:
    sw $a0, 112($0)
    jr $k0

syscall65:
    addi $sp, $sp, -8
    sw $t0, 0($sp)
    sw $t1, 4($sp)
    lw $t0, 48($0)
    lw $t1, 52($0)
    addi $v0, $t0, 0
    addi $v1, $t1, 0
    lw $t0, 0($sp)
    lw $t1, 4($sp)
    addi $sp, $sp, 8
    jr $k0


main:

addi $t1, $0, 1 #store 1 to monitor write
sw $t1, 44($0)

#array address A[i][j] is $sp + 64 * i + j
addi $sp, $sp, -16384

#s0 is xcoord
#s1 is ycoord
#s2 is xdirection
#s3 is ydirection

addi $s0, $0, 0
addi $s1, $0, 0
addi $s2, $0, 1
addi $s3, $0, 0
addi $s4, $0, 256 #color of the tron
addi $s5, $0, 64

addi $v0, $0, 5
syscall

addi $t6, $0, 1
beq $v0, $t6, draw_bar1

addi $t6, $0, 2
beq $v0, $t6, draw_bar2

addi $t6, $0, 3
beq $v0, $t6, draw_bar3

j endgame

draw_bar1:
    addi $t6, $sp, 0      #to store original sp
    addi $sp, $sp, -24
    sw $t0, 0($sp)
    sw $t1, 4($sp)
    sw $t4, 8($sp)
    sw $t5, 12($sp)
    sw $t2, 16($sp)
    sw $t3, 20($sp)
    addi $t0, $0, 10      #x-coordinate
    addi $t1, $0, 63      #y-coordinate
    addi $t4, $0, 255     #color of the wall
    addi $t5, $0, 40      #break point
    loop1:
        beq $t1, $t5, stop_draw_bar1    #when y-coord reached break point
        sll $t2, $t0, 6                 #multiply xcoord by 64
        add $t2, $t2, $t1               #t2 = 64 * t0 + t1
        add $t2, $t2, $t6               #add the stack pointer(in t6 now) to this
        sll $t2, $t2, 2                 #multiply by 4!
        addi $t3, $0, 1
        sw $t3, 0($t2)
        addi $v0, $0, 80
        add $a0, $0, $t0
        add $a1, $0, $t1
        add $a2, $0, $t4
        syscall
        addi $t1, $t1, -1
        j loop1
    stop_draw_bar1:
    lw $t0, 0($sp)
    lw $t1, 4($sp)
    lw $t4, 8($sp)
    lw $t5, 12($sp)
    lw $t2, 16($sp)
    lw $t3, 20($sp)
    addi $sp, $sp, 24

    j while

draw_bar2:
    addi $t6, $sp, 0      #to store original sp
    addi $sp, $sp, -24
    sw $t0, 0($sp)
    sw $t1, 4($sp)
    sw $t4, 8($sp)
    sw $t5, 12($sp)
    sw $t2, 16($sp)
    sw $t3, 20($sp)
    addi $t0, $0, 10      #x-coordinate
    addi $t1, $0, 63      #y-coordinate
    addi $t4, $0, 255     #color of the wall
    addi $t5, $0, 40      #break point
    loop1of2:
        beq $t1, $t5, stop_draw_bar1of2    #when y-coord reached break point
        sll $t2, $t0, 6                 #multiply xcoord by 64
        add $t2, $t2, $t1               #t2 = 64 * t0 + t1
        add $t2, $t2, $t6               #add the stack pointer(in t6 now) to this
        sll $t2, $t2, 2                 #multiply by 4!
        addi $t3, $0, 1
        sw $t3, 0($t2)
        addi $v0, $0, 80
        add $a0, $0, $t0
        add $a1, $0, $t1
        add $a2, $0, $t4
        syscall
        addi $t1, $t1, -1
        j loop1of2
    stop_draw_bar1of2:
    lw $t0, 0($sp)
    lw $t1, 4($sp)
    lw $t4, 8($sp)
    lw $t5, 12($sp)
    lw $t2, 16($sp)
    lw $t3, 20($sp)
    addi $sp, $sp, 24



    addi $t6, $sp, 0      #to store original sp
    addi $sp, $sp, -24
    sw $t0, 0($sp)
    sw $t1, 4($sp)
    sw $t4, 8($sp)
    sw $t5, 12($sp)
    sw $t2, 16($sp)
    sw $t3, 20($sp)
    addi $t0, $0, 63      #x-coordinate
    addi $t1, $0, 15      #y-coordinate
    addi $t4, $0, 255     #color of the wall
    addi $t5, $0, 35      #break point
    loop2of2:
        beq $t0, $t5, stop_draw_bar2of2    #when x-coord reached break point
        sll $t2, $t0, 6                 #multiply xcoord by 64
        add $t2, $t2, $t1               #t2 = 64 * t0 + t1
        add $t2, $t2, $t6               #add the stack pointer(in t6 now) to this
        sll $t2, $t2, 2                 #multiply by 4!
        addi $t3, $0, 1
        sw $t3, 0($t2)
        addi $v0, $0, 80
        add $a0, $0, $t0
        add $a1, $0, $t1
        add $a2, $0, $t4
        syscall
        addi $t0, $t0, -1
        j loop2of2
    stop_draw_bar2of2:
    lw $t0, 0($sp)
    lw $t1, 4($sp)
    lw $t4, 8($sp)
    lw $t5, 12($sp)
    lw $t2, 16($sp)
    lw $t3, 20($sp)
    addi $sp, $sp, 24

    j while

draw_bar3:
    addi $t6, $sp, 0      #to store original sp
    addi $sp, $sp, -24
    sw $t0, 0($sp)
    sw $t1, 4($sp)
    sw $t4, 8($sp)
    sw $t5, 12($sp)
    sw $t2, 16($sp)
    sw $t3, 20($sp)
    addi $t0, $0, 10      #x-coordinate
    addi $t1, $0, 63      #y-coordinate
    addi $t4, $0, 255     #color of the wall
    addi $t5, $0, 40      #break point
    loop1of3:
        beq $t1, $t5, stop_draw_bar1of3    #when y-coord reached break point
        sll $t2, $t0, 6                 #multiply xcoord by 64
        add $t2, $t2, $t1               #t2 = 64 * t0 + t1
        add $t2, $t2, $t6               #add the stack pointer(in t6 now) to this
        sll $t2, $t2, 2                 #multiply by 4!
        addi $t3, $0, 1
        sw $t3, 0($t2)
        addi $v0, $0, 80
        add $a0, $0, $t0
        add $a1, $0, $t1
        add $a2, $0, $t4
        syscall
        addi $t1, $t1, -1
        j loop1of3
    stop_draw_bar1of3:
    lw $t0, 0($sp)
    lw $t1, 4($sp)
    lw $t4, 8($sp)
    lw $t5, 12($sp)
    lw $t2, 16($sp)
    lw $t3, 20($sp)
    addi $sp, $sp, 24



    addi $t6, $sp, 0      #to store original sp
    addi $sp, $sp, -24
    sw $t0, 0($sp)
    sw $t1, 4($sp)
    sw $t4, 8($sp)
    sw $t5, 12($sp)
    sw $t2, 16($sp)
    sw $t3, 20($sp)
    addi $t0, $0, 63      #x-coordinate
    addi $t1, $0, 15      #y-coordinate
    addi $t4, $0, 255     #color of the wall
    addi $t5, $0, 35      #break point
    loop2of3:
        beq $t0, $t5, stop_draw_bar2of3   #when x-coord reached break point
        sll $t2, $t0, 6                 #multiply xcoord by 64
        add $t2, $t2, $t1               #t2 = 64 * t0 + t1
        add $t2, $t2, $t6               #add the stack pointer(in t6 now) to this
        sll $t2, $t2, 2                 #multiply by 4!
        addi $t3, $0, 1
        sw $t3, 0($t2)
        addi $v0, $0, 80
        add $a0, $0, $t0
        add $a1, $0, $t1
        add $a2, $0, $t4
        syscall
        addi $t0, $t0, -1
        j loop2of3
    stop_draw_bar2of3:
    lw $t0, 0($sp)
    lw $t1, 4($sp)
    lw $t4, 8($sp)
    lw $t5, 12($sp)
    lw $t2, 16($sp)
    lw $t3, 20($sp)
    addi $sp, $sp, 24


    addi $t6, $sp, 0      #to store original sp
    addi $sp, $sp, -24
    sw $t0, 0($sp)
    sw $t1, 4($sp)
    sw $t4, 8($sp)
    sw $t5, 12($sp)
    sw $t2, 16($sp)
    sw $t3, 20($sp)
    addi $t0, $0, 35      #x-coordinate
    addi $t1, $0, 50      #y-coordinate
    addi $t4, $0, 255     #color of the wall
    addi $t5, $0, 35      #break point
    loop3of3:
        beq $t1, $t5, stop_draw_bar3of3   #when x-coord reached break point
        sll $t2, $t0, 6                 #multiply xcoord by 64
        add $t2, $t2, $t1               #t2 = 64 * t0 + t1
        add $t2, $t2, $t6               #add the stack pointer(in t6 now) to this
        sll $t2, $t2, 2                 #multiply by 4!
        addi $t3, $0, 1
        sw $t3, 0($t2)
        addi $v0, $0, 80
        add $a0, $0, $t0
        add $a1, $0, $t1
        add $a2, $0, $t4
        syscall
        addi $t1, $t1, -1
        j loop3of3
    stop_draw_bar3of3:
    lw $t0, 0($sp)
    lw $t1, 4($sp)
    lw $t4, 8($sp)
    lw $t5, 12($sp)
    lw $t2, 16($sp)
    lw $t3, 20($sp)
    addi $sp, $sp, 24

    j while

    while:


    addi $v0, $0, 65
    syscall
    addi $t0, $0, 15 # t0 = maximum f
    addi $t3, $0, 1 # t3 = minimum 1

    bne $v0, $t0, endifW # go to endifW when x!=f
    bne $v1, $t3, endW # go to endW when y!=1
    ifW: #process W when (x,y) = (f,1)
        addi $s2, $0, 0
        addi $s3, $0, -1
    endifW:
    endW:

    bne $v0, $t3, endifA # go to endifA when x!=1
    bne $v1, $t3, endA # go to endA when y!=1
    ifA: #process A when (x,y) = (1,1)
        addi $s2, $0, -1
        addi $s3, $0, 0
    endifA:
    endA:

    bne $v0, $t0, endifD # go to endifD when x!=f
    bne $v1, $t0, endD # go to endD when y!=f
    ifD: #process D when (x,y) = (f,f)
        addi $s2, $0, 1
        addi $s3, $0, 0
    endifD:
    endD:

    bne $v0, $t3, endifS # go to endifS when x!=1
    bne $v1, $t0, endS # go to endS when y!=f
    ifS: #process S when (x,y) = (1,f)
        addi $s2, $0, 0
        addi $s3, $0, 1
    endifS:
    endS:

    add $s0, $s0, $s2
    add $s1, $s1, $s3

    sll $t0, $s0, 6 #multiply xcoord by 64
    add $t0, $t0, $s1 #t0 = 64 * s0 + s1
    add $t0, $t0, $sp #add the stack pointer to this
    sll $t0, $t0, 2 #multiply by 4!
    lw $t2, 0($t0) #load the array value
    addi $t1, $0, 1

    beq $t2, $t1, endgame #if that spot already visited, then end

    slt $t2, $s0, $0 #if xcoord less than zero we end (going off of map)
    beq $t2, $t1, endgame

    sge $t2, $s0, $s5 #if xcoord >= 64 we end (going off of map)
    beq $t2, $t1, endgame

    slt $t2, $s1, $0 #if ycoord less than zero we end (going off of map)
    beq $t2, $t1, endgame

    sge $t2, $s1, $s5 #if ycoord >= 64 we end (going off of map)
    beq $t2, $t1, endgame



    sw $t1, 0($t0)
    addi $v0, $0, 80
    add $a0, $0, $s0
    add $a1, $0, $s1
    add $a2, $0, $s4
    syscall



    j while



endgame:
    addi $sp, $sp, 16384 #return stack pointer to neutral

    addi $v0, $0, 10
    syscall
