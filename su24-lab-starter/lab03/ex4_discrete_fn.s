.globl f # this allows other files to find the function f
# NOT allowed to use any branch or jump instructions
# f takes in two arguments:
# a0 is the value we want to evaluate f at
# a1 is the address of the "output" array (read the lab spec for more information).
# The return value should be stored in a0
f:
    # Your code here
    addi a2, x0, 3  # input from -3 to 3
    add  a0, a0, a2 # a0 = a0 + 3, the index of the output array
    slli a0, a0, 2  # a0 = a0 * 4, the offset of the output array
    add  a1, a0, a1 # a1 = a0 + a1, the address of the output array
    lw   a0, 0(a1)  # a0 = output[a0]
    # This is how you return from a function. You'll learn more about this later.
    # This should be the last line in your program.
    jr ra
