.globl factorial

.data
n: .word 7

.text
# Don't worry about understanding the code in main
# You'll learn more about function calls in lecture soon
main:
    la t0, n
    lw a0, 0(t0)
    jal ra, factorial

    addi a1, a0, 0
    addi a0, x0, 1
    ecall # Print Result

    addi a1, x0, '\n'
    addi a0, x0, 11
    ecall # Print newline

    addi a0, x0, 10
    ecall # Exit

# factorial takes one argument:
# a0 contains the number which we want to compute the factorial of
# The return value should be stored in a0
factorial:
    # use a2 to store the result temporarily
    addi a2, x0, 1 # a2 = 1
out_loop:
    beq  a0, x0, end # if a0 == 0, jump to end
    add  a3, x0, a0  # a3 = a0
    add  a4, x0, a2  # a4 = a2

    inner_loop:
        addi a3, a3, -1 # a3 -= 1
        beq  a3, x0, inner_end  # if a3 = 0
        add  a2, a2, a4 # a2 += a4
        j    inner_loop
    inner_end:
        addi a0, a0, -1 # a0 -= 1

    j   out_loop
end:
    add  a0, a2, x0
    # This is how you return from a function. You'll learn more about this later.
    # This should be the last line in your program.
    jr ra
