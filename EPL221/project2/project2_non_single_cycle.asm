.data

A: .word 11 22 33 44 55 66 77 88 99 110
SIZE: .word 10

new_line: .asciiz "\n"

msg0: .asciiz "The Sum of the Table is: "

.text
    main:
        #lw $s0,A($0)
        addi $s3,$0,10

        la $a0, msg0
        addi $v0, $0,4
        syscall

        #Call the rSum:
        addi $s5,$0,1
        jal rSum
        ###############
        addi $v0,$0, 1
        move $a0, $s5
        syscall

        ### call the print#####
        jal priont_table
        #######################

        li $v0,10
        syscall
    ###Arguments#####
    # A -> $s0
    # n -> $s3
    # reuturn $s5 (total sum).
    rSum:
        addi $sp,$sp,-12
        sw $ra,8($sp) # store the return point
        sw $s3,4($sp) # store the n.(every recursion it will store the n-1).

        base_case:
            beq $0,$s3, breake_rSum
        
        lw $s3, 4($sp)
        addi $s3,$s3,-1 # n=n-1.
        jal rSum

        lw $s3,4($sp)
        
        add $t7,$s3,$s3 # n + n = 2n
        add $t7,$t7, $t7    # 2n + 2n = 4n.
        add $t4, $t7, $0
        lw $t3, A($t4)
        add $s5,$s5,$t3 # sum += A[i];
        beq $0,$0 end_rSum

        breake_rSum:
            addi $s5,$0,1

        end_rSum:
            lw $ra, 8($sp) # bring the return addres from mem.
            addi $sp,$sp,12
            jr $ra 
    

    #void printTable(unsigned int A[], int n){
    #arguments: none
    priont_table:
        add $t0,$0,$0
        loop:
            #new line
            la $a0,new_line
            addi $v0,$0,4
            syscall
            ##############
            addi $t1,$0,10
            beq $t0,$t1,break_loop

            add $t7,$t0,$t0
            nop
            add $t7,$t7,$t7

            lw $t3,A($t7)
            move $a0,$t3
            addi $v0,$0,1
            syscall
            addi $t0,$t0,1

            
            beq $0,$0,loop
        break_loop:
        jr $ra
