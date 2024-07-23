.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int arrays
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the number of elements to use
#   a3 (int)  is the stride of arr0
#   a4 (int)  is the stride of arr1
# Returns:
#   a0 (int)  is the dot product of arr0 and arr1
# Exceptions:
#   - If the number of elements to use is less than 1,
#     this function terminates the program with error code 36
#   - If the stride of either array is less than 1,
#     this function terminates the program with error code 37
# =======================================================
dot:
    ble  a2, x0, error_case
    ble  a3, x0, error_case2
    ble  a4, x0, error_case2
    # Prologue
    add  t2, x0, x0
    addi t2, t2, 4  # t2 = 4
    mul  t0, a3, t2 # the address stride of arrar0
    mul  t1, a4, t2 # the address stride of array1
    add  t2, x0, x0 # the loop counter
    add  t3, a0, x0 # t3 is the array0 pointer
    add  t4, a1, x0 # t4 is the array1 pointer
    addi a0, x0, 0  # now a0 is to store the retur value
    # t5 is to load array0's element, t6 is to array1's
    # t5 is to store the result of product
loop_start:
    beq  t2, a2, loop_end
    addi t2, t2, 1  # t2 += 1
    lw   t5, 0(t3)  # load array0
    lw   t6, 0(t4)  # load array1
    mul  t5, t5, t6 # t5 = t5 * t6
    add  a0, a0, t5 # a0 += result
    add  t3, t3, t0
    add  t4, t4, t1 # update the pointer
    j    loop_start
loop_end:
    # Epilogue
    jr ra
error_case:
    li   a0, 36             # generate error code
    j    exit               # exit the program
error_case2:
    li   a0, 37             # generate error code
    j    exit               # exit the program