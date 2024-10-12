.data
    msg1: .asciiz "\nterms["
    msg11: .asciiz "] : ln("  
    msg12: .asciiz ")="
    
    msg21: .asciiz "]R: ln("
    
.text
    main:
    j global_main
        ####Arguments###
        # $f12 --> x
        # $a1  --> n
        ###Return####
        # retrun -> $f0
        powerR:
            addi $sp, $sp, -16      # allocate space for return addres , n, x, and xˆ (for after recursion).
            sw $ra, 12($sp)         # save the return addres to the stack.
            sw $a1, 8($sp)          # save the a1 (n) to the stack (for recursion).
            swc1 $f12, 4($sp)       # save at the c1 the value of the x.

            # Base case
            beq $a1, $zero, base_case   # check if n==0, if true go to base_case.

            addi $a1, $a1, -1       # n-1;
            swc1 $f12, 0($sp)       # save x before recursion.
            jal powerR              # call the recursion.
            lwc1 $f12, 0($sp)       # restore the x after finishing recursion.

            mul.s $f0, $f0, $f12    # store at the $f0, the x*recursion(x,n-1).
            j end_powerR            # goto end_powerR

        base_case:
            li.s $f0, 1.0           # make the return register $f0 hold the value 1.0 (return 1.0).

        end_powerR:
            lw $ra, 12($sp)         # bring from stack the return address.
            lw $a1, 8($sp)          # bring the past n.
            lwc1 $f12, 4($sp)       # bring the past x.
            addi $sp, $sp, 16       # delocate stack space.
            jr $ra                  # return.

        ####Arguments###
        # float x --> $f12
        # int i   --> $a1
        # int n   --> $a2
        Taylor_lnR:
            add $sp, $sp, -20                   # allocate space in stack for 5.
            sw $ra, 16($sp)                     # save in stack the return address.
            sw $a1, 12($sp)                     # save the i ($a1).
            sw $a2, 8($sp)                      # save the n ($a2).
            swc1 $f12, 4($sp)                   # save the float x ($f12).
            swc1 $f20, 0($sp)                   # save the total return value ($f20).

            # Base case
            bgt $a1, $a2, Taylor_lnR_base_case  #if a1>a2 goto Taylor_lnR_base_case.

            ### powerR(-1, i+1) ###
            li.s $f12, -1.0
            addi $a1, $a1, 1
            jal powerR
            mov.s $f17, $f0    # return value stored at $f17.
            ########################

            ### powerR(x,i) ###
            lwc1 $f12, 4($sp)
            addi $a1, $a1, -1
            jal powerR
            mov.s $f18, $f0     # return value stored at $f18.
            ###################

            ### powerR(x,i)/i ###
            mtc1 $a1, $f22
            cvt.s.w $f22, $f22
            div.s $f19, $f18, $f22
            ######################

            ### powerR(x,i)*powerR(x,i)/i
            mul.s $f20, $f17, $f19


            # ++i
            addi $a1, $a1, 1

            # call recursion
            jal Taylor_lnR

            # add it to the return register($f0).
            add.s $f0, $f0, $f20

            # load registers from stack.
            lw $ra, 16($sp)
            lw $a1, 12($sp)
            lw $a2, 8($sp)
            lwc1 $f12, 4($sp)
            lwc1 $f20, 0($sp)
            add $sp, $sp, 20    # free space in stack.
            jr $ra      # return

        Taylor_lnR_base_case:
            li.s $f0, 0.0
            lw $ra, 16($sp)
            add $sp, $sp, 20
            jr $ra

        ###Arguments###
        # float x --> $f12
        # int n   --> $a1
        ###Return###
        # $f0
        Taylor_lnRecursive:
            addi $sp, $sp, -4       # allocate 4 bytes on the stack.
            sw $ra, 0($sp)          # save return address.
            move $a2,$a1            # add the n to the a2 for Taylor_lnR. (3rd argument).    
            li $a1, 1               # add the value of 1 to the a1.(2nd argument).
            li.s $f20, -1.0         # make $f20 have value -1.0.
            add.s $f12,$f12,$f20    # do the x-1 and save it in $f12 argument register. (1st argument).
            jal Taylor_lnR          # call the method.
            lw $ra, 0($sp)          # load the return address
            addi $sp, $sp, 4        # free up stack space
            jr $ra                  # return.


        ### Arguments ###
        #f12 -> x
        #a0 -> n
        Taylor_ln:
            ##############Variables in Use ###########
            #   $t0 -> int i
            #   $f1 -> float sum = 0,0
            #   $t1 -> n(argument) passed through $a0.
            #   $f2 -> x(argument) passed through $f12.
            ##########################################
    
            addi $sp, $sp, -4        # make space on the stack.
            sw $ra, 0($sp)           # save return address.
            mov.s $f2,$f12           # copy the value of f0, to f2 , bacause arguments are passed by value NOT by reference.
            move $t1,$a0             # copy the value of n($a0) to new n($t1).

            li $t0,0                 # int i.
            li.s $f1, 0.0            # create float sum($f1) = 0.0.

            # x=x-1.
            li.s $f3, 1.0       
            sub.s $f2, $f2,$f3  # x = x - 1.

            #set the counter to 1.
            li $t0,1    #loop counter i.
            Taylor_ln_loop:
                ####reset the f12 and $f7 to zero#######
                mtc1 $zero, $f12
                mtc1 $zero, $f7  #f7 = 0
                ################################

                ####powerR(-1,i+1)##########
                li.s $f12, -1.0             # add the $f12 the value of the -1.0. (1st argument) --$f12.
                move $t5,$t0                # copy the i to the $t5.
                addi $t5,$t5,1              # do the i+1.
                move $a1, $t5               # copy the i($t0) variable to the $a0.( 2nd argument) --$a1.
                ##caling the method##
                jal powerR                  # call the function powerR.
                mov.s $f7, $f0              # add the return value of $f0 to $f7.
                #########################

                ####reset the f12 and $f6 to zero#######
                mtc1 $zero, $f12
                mtc1 $zero, $f6  #f6 = 0
                ################################

                ## powerR(x,i)/i #############
                mov.s $f12,$f2          # copy the value of X ($f2) to the $f12. (1st argument) -- $f12.
                move $a1, $t0           # copy the value of I ($t0) to the $a1. (2nd argument) -- $a1.
                jal powerR
                mov.s $f6, $f0          # add the return value of $f0 to $f6     -> $f6 = $f12
                mtc1 $t0,$f5            # convert the i into the float $f5
                cvt.s.w $f5,$f5         # convert proccess.
                div.s $f6,$f6,$f5       # $f6/f5 --> powerR(x,i)/i

                # multiply those two
                mul.s $f5, $f6,$f7      # f5 = f6*f7 
                add.s $f1,$f1,$f5       # sum = sum + powerR * powerR/i

                 ####reset the f12 to zero#######
                mtc1 $zero, $f12
                ################################
                beq $t0,$t1,Taylor_ln_loop_break    # if $t0 == $t1 == n goto break the loop.
                ####Increase the counter#######
                addi $t0,$t0,1
                j Taylor_ln_loop

            Taylor_ln_loop_break:
                mov.s $f0, $f1      # move the sum to $f12 argument register
                lw $ra, 0($sp)       # Restore return address
                addi $sp, $sp, 4     # Free up stack space
                jr $ra

    global_main:
        #Variables in use:
        # $f29 -> float i.
        # $s0 -> int n.
        li.s $f29, 0.0   # initialise the float i to $f29.
        li $s0, 100     # int n = 100; // The number of t.
        main_loop:
            #*****Main Exection******#

            #print output for Taylor_ln
                                   #print "temrms" 
            li $v0, 4               # Load the syscall code for printing string
            la $a0, msg1
            syscall
                                    # print the n value
            li $v0,1
            move $a0, $s0 
            syscall 
                                    #print "]: ln("
            li $v0, 4               # Load the syscall code for printing string
            la $a0, msg11
            syscall
                                    # print the i value
            li $v0, 2               # Load the syscall code for printing float (2)
            mov.s $f12, $f29         # Load the float value from memory into $f12
            syscall 
                                    #print ")="
            li $v0, 4               # Load the syscall code for printing string
            la $a0, msg12
            syscall

            ###############Call Taylor_ln##########################
            move $a0, $s0   # move the value for $s0 to the $a0, give n as an argument.
            mov.s $f12, $f29# move the value of i to the $f12 argument register.
            jal Taylor_ln   # call the Taylor_ln function.
            ######################################################
            li $v0, 2               # Load the syscall code for printing float (2)
            mov.s $f12, $f0         # Load the float value from memory into $f12
            syscall 

            mtc1 $zero, $f0         # reset the retrun value $f0.
            # print for Taylor_lnRecursive ######
            li $v0, 4               # Load the syscall code for printing string
            la $a0, msg1
            syscall
                                    # print the n value
            li $v0,1
            move $a0, $s0 
            syscall 
                                    #print "]: ln("
            li $v0, 4               # Load the syscall code for printing string
            la $a0, msg21
            syscall
                                    # print the i value
            li $v0, 2               # Load the syscall code for printing float (2)
            mov.s $f12, $f29         # Load the float value from memory into $f12
            syscall 
                                    #print ")="
            li $v0, 4               # Load the syscall code for printing string
            la $a0, msg12
            syscall


            ##############Call Taylor_lnRecursive##########################
            move $a1, $s0   # move the value for $s0 to the $a1, give n as an argument.
            mov.s $f12, $f29# move the value of i to the $f12  x argument register.
            jal Taylor_lnRecursive   # call the Taylor_ln function.
            #############################################

            li $v0, 2               # Load the syscall code for printing float (2)
            mov.s $f12, $f0          # Load the float value from thereturn of the function into $f12
            syscall 
#
            #############
            move $a0, $s0
            cvt.w.s $f31, $f29      # Convert the float in $f29 to an integer, store it in $f31
            mfc1 $t0, $f31          # save the integer amout to the $t0.
            li $t5,2                # assign the value of 2 to the $t5. to compare after.
            beq $t0,$t5,break_main_loop     #check if the counter is equal to 2.
            #************************#
            #increase the counter by 1
            li.s $f1 0.1    #create a temp variable at $f1 to store the value of 0.1
            add.s $f29,$f29,$f1   # do $f29= $f29 + $f1 where $f1= 0.1 (i+=0.1)
            #************************#
            j main_loop


        break_main_loop:
            li $v0, 10
 	    	syscall



    li $v0, 2               # Load the syscall code for printing float (2)
    mov.s $f12, $f0     # Load the float value from memory into $f12
    syscall 
