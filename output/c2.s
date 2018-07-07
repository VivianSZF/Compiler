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

fact:
  subu $sp, $sp, 0
  sw $fp, -4($sp)
  addi $fp, $sp, 0
  sw $a0, -8($fp)
  lw $t0, -8($fp)
  li $t1, 1
  beq $t0, $t1, l0
  j l1
l0:
  lw $t0, -8($fp)
  lw $fp, -4($sp)
  addi $sp, $sp, 0
  move $v0, $t0
  jr $ra
  j l2
l1:
  lw $t0, -12($fp)
  lw $t1, -8($fp)
  addi $t0, $t1, -1
  sw $t0, -12($fp)
  lw $t0, -12($fp)
  move $a0, $t0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal fact
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $v0, -16($fp)
  lw $t0, -20($fp)
  lw $t1, -8($fp)
  lw $t2, -16($fp)
  mul $t0, $t1, $t2
  sw $t0, -20($fp)
  lw $t0, -20($fp)
  lw $fp, -4($sp)
  addi $sp, $sp, 0
  move $v0, $t0
  jr $ra
l2:

main:
  subu $sp, $sp, 0
  sw $fp, -4($sp)
  addi $fp, $sp, 0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $v0, -8($fp)
  lw $t0, -8($fp)
  sw $t0, -12($fp)
  lw $t0, -12($fp)
  li $t1, 1
  bgt $t0, $t1, l3
  j l4
l3:
  lw $t0, -12($fp)
  move $a0, $t0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal fact
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $v0, -16($fp)
  lw $t0, -16($fp)
  sw $t0, -20($fp)
  j l5
l4:
  li $t0, 1
  sw $t0, -20($fp)
l5:
  lw $a0, -20($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  li $t0, 0
  lw $fp, -4($sp)
  addi $sp, $sp, 0
  move $v0, $t0
  jr $ra
