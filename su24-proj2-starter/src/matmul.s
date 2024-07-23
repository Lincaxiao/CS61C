.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
#   d = matmul(m0, m1)
# Arguments:
#   a0 (int*)  is the pointer to the start of m0
#   a1 (int)   is the # of rows (height) of m0
#   a2 (int)   is the # of columns (width) of m0
#   a3 (int*)  is the pointer to the start of m1
#   a4 (int)   is the # of rows (height) of m1
#   a5 (int)   is the # of columns (width) of m1
#   a6 (int*)  is the pointer to the the start of d
# Returns:
#   None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 38
# =======================================================
matmul:

    # Error checks
    li t0, 1
    blt a1, t0, bad
    blt a2, t0, bad
    blt a4, t0, bad
    blt a5, t0, bad
    bne a2, a4, bad

    # Prologue


outer_loop_start:
    mv t1, a0 # t1: arr0
    li t0, 4
    mul t0, t0, a2 # t0 = width * 4 = offset
    li t3, 1 # out times


inner_loop_start:
    mv t2, a3 # t2: arr1
    li t4, 1 #inner times
    # for inner loop, the offset is always 4
    mv t5, a6 # t5: offset of arr d
    
outer_loop_continue:
    bgt t3, a1, outer_loop_end

inner_loop_continue:
    bgt t4, a5, inner_loop_end

    # call 函数之前是否要保存参数,答案是要的
    # inner prologue，这个位置是否需要变动？
    addi sp, sp, -64       
    sw   ra, 60(sp)        # 保存返回地址寄存器 ra
    sw   t0, 56(sp)        # 保存临时寄存器 t0
    sw   t1, 52(sp)        # 保存临时寄存器 t1
    sw   t2, 48(sp)        # 保存临时寄存器 t2
    sw   t3, 44(sp)        # 保存临时寄存器 t3
    sw   t4, 40(sp)        # 保存临时寄存器 t4
    sw   t5, 36(sp)        # 保存临时寄存器 t5
    sw   t6, 32(sp)        # 保存临时寄存器 t6
    sw   a0, 28(sp)        # 保存参数寄存器 a0
    sw   a1, 24(sp)        # 保存参数寄存器 a1
    sw   a2, 20(sp)        # 保存参数寄存器 a2
    sw   a3, 16(sp)        # 保存参数寄存器 a3
    sw   a4, 12(sp)        # 保存参数寄存器 a4
    sw   a5, 8(sp)         # 保存参数寄存器 a5
    sw   a6, 4(sp)         # 保存参数寄存器 a6
    sw   a7, 0(sp)         # 保存参数寄存器 a7

    mv a0, t1
    mv a1, t2
    # a2 可以不变
    li a3, 1
    # a4 也可以不变
    mv a4, a5
    call dot
    # put res in arr d
    # need first restore t5
    lw t5, 36(sp)
    sw a0, 0(t5)


    # inner epilogue
    lw   ra, 60(sp)        # 恢复返回地址寄存器 ra
    lw   t0, 56(sp)        # 恢复临时寄存器 t0
    lw   t1, 52(sp)        # 恢复临时寄存器 t1
    lw   t2, 48(sp)        # 恢复临时寄存器 t2
    lw   t3, 44(sp)        # 恢复临时寄存器 t3
    lw   t4, 40(sp)        # 恢复临时寄存器 t4
    lw   t5, 36(sp)        # 恢复临时寄存器 t5
    lw   t6, 32(sp)        # 恢复临时寄存器 t6
    lw   a0, 28(sp)        # 恢复参数寄存器 a0
    lw   a1, 24(sp)        # 恢复参数寄存器 a1
    lw   a2, 20(sp)        # 恢复参数寄存器 a2
    lw   a3, 16(sp)        # 恢复参数寄存器 a3
    lw   a4, 12(sp)        # 恢复参数寄存器 a4
    lw   a5, 8(sp)         # 恢复参数寄存器 a5
    lw   a6, 4(sp)         # 恢复参数寄存器 a6
    lw   a7, 0(sp)         # 恢复参数寄存器 a7
    addi sp, sp, 64        # 恢复栈指针


    # 递增，下一次循环
    addi t2, t2, 4
    addi t4, t4, 1
    addi t5, t5, 4

    j inner_loop_continue


inner_loop_end:

    addi t3, t3, 1
    add t1, t1, t0

    # reset inner times and arr2
    li t4, 1
    mv t2, a3

    j outer_loop_continue

outer_loop_end:

    # Epilogue

    jr ra

bad:
    li a0, 38
    j exit