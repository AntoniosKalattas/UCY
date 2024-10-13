.data
A: .space 400

.text
    main:
    add $t0,$0,$0  # $t0 coutner for the loop aka int i.
    loop:
        addi $t5,$0,100     # if i==100 break the loop.
        beq $t5,$t0,break_main_loop 
        add $t4, $t0,$t0    # $t4 = i+i.
        add $t4,$t4,$t4     # |
        sw $t4, A($t4)       # Store $t4 at the calculated address
        addi $t0,$t0,1
        beq $0,$0,loop
        

    break_main_loop:
        li $v0, 10
 	    	syscall