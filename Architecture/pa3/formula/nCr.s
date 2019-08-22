/*
 * nCr.s
 * Anirudh Tunoori netid: at813
 */

## PLEASE IGNORE THE EXCESS/REDUNDANT COMMENTS\\
.file	"nCr.c"
.text

## nCr function
.globl nCr
        .type   nCr, @function
nCr:
  # Your code for nCr should go here
    pushl	%ebp   ## push the base frame pointer (function entry)
    movl	%esp, %ebp  ## copy the contents of the stack frame pointer to the base frame pointer (esp contains n among other things)
    pushl   %ebx        ## push the contents of %ebx onto the stack
    subl    $36, %esp   ## reserve space (36 bytes) on the stack frame
    movl	8(%ebp), %eax  ## Read the adress in %ebp, add 8 to it and move the obtained reult/address to %eax
    movl	%eax, (%esp) ## copy the contents of eax(which happens to be an address) and move it to the memory
                         ## location whose address is in the stack pointer (prepare for function call)

    call	Factorial   ## Call the Factorial function (the formula is applied once alll three factorial components are obtained)
    movl	%eax, -16(%ebp) ## copy the contents of %eax (n!) into a memory location 16 bytes away from the base frame pointer: %ebp
    movl	12(%ebp), %eax ## Read the address in %ebp, add 12 to it and move the obtained reult/address to %eax
    movl	%eax, (%esp)   ## copy the contents of eax(which happens to be an address) and move it to the memory
                        ## location whose address is in the stack pointer (prepare for function call)

    call	Factorial   ## Factorial function call#2
    movl	%eax, %ebx  ## obtain r! and move it into %ebx
    movl	12(%ebp), %eax
    movl	8(%ebp), %edx
    movl	%edx, %ecx
    subl	%eax, %ecx  ## subtraction (n-r) in order to latter obtain (n-r)! insert [contentsof(%ecx aka n) - contentsof(%eax aka r)] into %ecx
    movl	%ecx, %eax  ## move (n-r) into the eax register
    movl	%eax, (%esp) ## place/copy (n-r) into the appropriate address within the stack pointer (prepare for function call)

    call	Factorial   ## Factorial function call#3 obtain (n-r)!
    imull	%ebx, %eax  ## perform unsigned multiplication: r!(n-r)!
    movl	%eax, -12(%ebp) ## copy the contents of %eax (r!(n-r)!) into a memory location 12 bytes away from the base frame pointer
    cmpl	$0, -16(%ebp) ## compare the contents in the mem location that is 16 bytes away from the base frame pointer to the value: 0
    je	.L9                 ## true branch of the "condition"
    cmpl	$0, -12(%ebp)
    jne	.L10                ## jump not equal

.L9:    ## jump equal
    movl	$0, %eax
    jmp	.L11
.L10:  ## jump not equal
    movl	-16(%ebp), %eax
    movl	%eax, %edx
    sarl	$31, %edx       ## right shift/signed division by a count of 31 and store the result in %edx
    idivl	-12(%ebp)      ## perform the (signed) division aspect of the formula
.L11:   ## return control to the main flow of the function
    addl	$36, %esp    ## remove space (the same 36 bytes that were reserved in the beginning of the function) from the stack pointer
    popl	%ebx            ## clear the contents of %ebp (the actual vaue is preserved/returned)
    popl	%ebp            ## pop the base frame (exit the function)

    ret                 ## function exit
    .size	nCr, .-nCr


## Factorial function
.globl Factorial
        .type   Factorial, @function
Factorial:
  # Your code for Factorial should go here
    pushl	%ebp    ## push the base frame pointer (function entry)
    movl	%esp, %ebp  ## copy the contents of the stack pointer to the frame pointer (esp contains n among other things)
    subl    $12, %esp   ## reserve space (12 bytes) on the stack frame
    movl	8(%ebp), %eax   ## Read the adress in %ebp, add 8 to it and move the obtained reult/address to %eax
    movl	%eax, -4(%ebp)  ## copy the contents of %eaxinto a memory location 4 bytes away from the base frame pointer: %ebp
    movl	$1, -12(%ebp)   ## "insert" 1 into a location 12 bytes(three ints) away from the address of the base frame pointer
    movl	$1, -8(%ebp)    ## "insert" 1 into a location 8 bytes(two ints) away from the address of the base frame pointer

LBB0_1:   ## Loop:
    movl	-8(%ebp), %eax
    cmpl	-4(%ebp), %eax      ## loop condition compare the index with "n"
    jg	LBB0_4                          ## exit the loop (index > "n")
    movl	-12(%ebp), %eax
    imull	-8(%ebp), %eax      ## Do the multiplication operation and store the result in %eax
    movl	%eax, -12(%ebp)
    movl	-8(%ebp), %eax
    addl	$1, %eax            ## increment the index (counter)
    movl	%eax, -8(%ebp)
    jmp	LBB0_1                  ## return back to the begiinning of the loop (loop condition)

LBB0_4: ## "leave"/function exit: movl %ebp, %esp and popl %ebp
    movl	-12(%ebp), %eax
    addl	$12, %esp
    popl	%ebp
    ret     ## official exit
    .size	Factorial, .-Factorial

.ident	"GCC: (GNU) 4.1.2 20080704 (Red Hat 4.1.2-51)"
.section	.note.GNU-stack,"",@progbits
