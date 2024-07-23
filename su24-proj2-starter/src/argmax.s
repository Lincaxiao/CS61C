.globl argmax

.text
# =================================================================
# FUNCTION: Given a int array, return the index of the largest
#   element. If there are multiple, return the one
#   with the smallest index.
# Arguments:
#   a0 (int*) is the pointer to the start of the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   a0 (int)  is the first index of the largest element
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# =================================================================
argmax:
    # Prologue
    ble  a1, x0, error_case
    add  t0, a0, x0  # t0 = a0
    add  t1, x0, x0  # t1 = 0, loop counter
    add  t2, x0, x0  # t2 = 0, store the maximum number
    add  t3, x0, x0  # t3 = 0, store the index
loop_start:
    beq  t1, a1, loop_end
    lw   t4, 0(t0)   # load the element
    addi t1, t1, 1  # t1 += 1
    addi t0, t0, 4   # t0 += 4, next address
    ble  t4, t2, loop_start
loop_continue:
    add  t2, t4, x0  # t2 <- t4, update the max value
    addi t3, t1, -1  # update the index
    j    loop_start
loop_end:
    # Epilogue
    add  a0, t3, x0  # return value
    jr ra
error_case:
    li   a0, 36             # generate error code
    j    exit               # exit the program
