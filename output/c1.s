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

main:
  subu $sp, $sp, 0
  sw $fp, -4($sp)
  addi $fp, $sp, 0
  li $t0, 0
  sw $t0, -8($fp)
  li $t0, 1
  sw $t0, -12($fp)
  li $t0, 0
  sw $t0, -16($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $v0, -20($fp)
  lw $t0, -20($fp)
  sw $t0, -24($fp)
l0:
  lw $t0, -16($fp)
  lw $t1, -24($fp)
  blt $t0, $t1, l1
  j l2
l1:
  lw $t0, -28($fp)
  lw $t1, -8($fp)
  lw $t2, -12($fp)
  add $t0, $t1, $t2
  sw $t0, -28($fp)
  lw $t0, -28($fp)
  sw $t0, -32($fp)
  lw $a0, -12($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  lw $t0, -12($fp)
  sw $t0, -8($fp)
  lw $t0, -32($fp)
  sw $t0, -12($fp)
  lw $t0, -36($fp)
  lw $t1, -16($fp)
  addi $t0, $t1, 1
  sw $t0, -36($fp)
  lw $t0, -36($fp)
  sw $t0, -16($fp)
  j l0
l2:
  li $t0, 0
  lw $fp, -4($sp)
  addi $sp, $sp, 0
  move $v0, $t0
  jr $ra
