.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
  li $v0, 4
  la $a0, _prompt
  syscall
  li $v0, 5
  syscall
  jr $ra
write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _ret
  syscall
  move $v0, $0
  jr $ra

Add:
  sw $fp, -4($sp)
  move $fp, $sp
  addi $sp, $sp, -48
  sw $a0, -8($fp)
  sw $a1, -12($fp)
  sw $a2, -16($fp)
  sw $a3, -20($fp)
  lw $t0, -24($fp)
  lw $t1, -8($fp)
  lw $t2, -12($fp)
  add $t0, $t1, $t2
  sw $t0, -24($fp)
  lw $t0, -28($fp)
  lw $t1, -24($fp)
  lw $t2, -16($fp)
  add $t0, $t1, $t2
  sw $t0, -28($fp)
  lw $t0, -32($fp)
  lw $t1, -28($fp)
  lw $t2, -20($fp)
  add $t0, $t1, $t2
  sw $t0, -32($fp)
  lw $t0, -36($fp)
  lw $t1, -32($fp)
  lw $t2, 4($fp)
  add $t0, $t1, $t2
  sw $t0, -36($fp)
  lw $t0, -40($fp)
  lw $t1, -36($fp)
  lw $t2, 8($fp)
  add $t0, $t1, $t2
  sw $t0, -40($fp)
  lw $t0, -44($fp)
  lw $t1, -40($fp)
  lw $t2, 12($fp)
  add $t0, $t1, $t2
  sw $t0, -44($fp)
  lw $t0, -44($fp)
  sw $t0, -48($fp)
  lw $t0, -48($fp)
  addi $sp, $sp, 48
  lw $fp, -4($sp)
  move $v0, $t0
  jr $ra


main:
  sw $fp, -4($sp)
  move $fp, $sp
  addi $sp, $sp, -44
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $v0, -8($fp)
  lw $t0, -8($fp)
  sw $t0, -12($fp)
  li $t0, 1
  sw $t0, -16($fp)
  li $t0, 2
  sw $t0, -20($fp)
  li $t0, 1
  sw $t0, -24($fp)
  li $t0, 4
  sw $t0, -28($fp)
  li $t0, 5
  sw $t0, -32($fp)
  li $t0, 10
  sw $t0, -36($fp)
  addi $sp, $sp, -12
  lw $t0, -12($fp)
  move $a0, $t0
  lw $t1, -16($fp)
  move $a1, $t1
  lw $t2, -20($fp)
  move $a2, $t2
  lw $t3, -24($fp)
  move $a3, $t3
  lw $t4, -28($fp)
  sw $t4, 0($sp)
  lw $t5, -32($fp)
  sw $t5, 4($sp)
  lw $t6, -36($fp)
  sw $t6, 8($sp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal Add
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  addi $sp, $sp, 12
  sw $v0, -40($fp)
  lw $t0, -40($fp)
  sw $t0, -44($fp)
  lw $a0, -44($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  li $t0, 0
  addi $sp, $sp, 44
  lw $fp, -4($sp)
  move $v0, $t0
  jr $ra

