.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
#   a0 (int*) is the pointer to the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   None
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# ==============================================================================
relu:
    # Prologue
    # deal with the error case, if a1 <= 0
    ble  a1, x0, error_case
    add  t0, x0, a0         # array pointer
    add  t1, x0, a1         # loop counter
loop_start:
    ble  t1, x0, loop_end   # if t0 = 0, finish the loop
    lw   t3, 0(t0)          # load the array element   
    addi t0, t0, 4          # next element's address
    addi t1, t1, -1         # counter -= 1
    blt  t3, x0, loop_continue # if t3 < 0, turn it to 0
    j    loop_start         # next loop
loop_continue:
    sw   x0, -4(t0)         # turn negative value to 0
    j    loop_start         # next loop
loop_end:
    # Epilogue
    jr ra
error_case:
    li   a0, 36             # generate error code
    j    exit               # exit the program
    