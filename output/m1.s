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
  addi $sp, $sp, -24
  sw $a0, -8($fp)
  sw $a1, -12($fp)
  sw $a2, -16($fp)
  lw $t0, -8($fp)
  lw $t1, -12($fp)
  bgt $t0, $t1, l0
  j l1
l0:
  lw $t0, -20($fp)
  lw $t1, -8($fp)
  lw $t2, -12($fp)
  sub $t0, $t1, $t2
  sw $t0, -20($fp)
  lw $t0, -20($fp)
  addi $sp, $sp, 24
  lw $fp, -4($sp)
  move $v0, $t0
  jr $ra

l1:
  lw $t0, -8($fp)
  lw $t1, -16($fp)
  bgt $t0, $t1, l2
  j l3
l2:
  lw $t0, -24($fp)
  lw $t1, -8($fp)
  lw $t2, -16($fp)
  sub $t0, $t1, $t2
  sw $t0, -24($fp)
  lw $t0, -24($fp)
  addi $sp, $sp, 24
  lw $fp, -4($sp)
  move $v0, $t0
  jr $ra

l3:
  lw $t0, -16($fp)
  addi $sp, $sp, 24
  lw $fp, -4($sp)
  move $v0, $t0
  jr $ra


main:
  sw $fp, -4($sp)
  move $fp, $sp
  addi $sp, $sp, -36
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $v0, -8($fp)
  lw $t0, -8($fp)
  sw $t0, -12($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $v0, -16($fp)
  lw $t0, -16($fp)
  sw $t0, -20($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $v0, -24($fp)
  lw $t0, -24($fp)
  sw $t0, -28($fp)
  lw $t0, -12($fp)
  move $a0, $t0
  lw $t1, -20($fp)
  move $a1, $t1
  lw $t2, -28($fp)
  move $a2, $t2
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal Add
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $v0, -32($fp)
  lw $t0, -32($fp)
  sw $t0, -36($fp)
  lw $a0, -36($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  li $t0, 0
  addi $sp, $sp, 36
  lw $fp, -4($sp)
  move $v0, $t0
  jr $ra

