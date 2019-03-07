	.text	
title: .string "Keyboard Demo - OpenGL"
	.global main
main:
	# rdi,rsi,rdx,rcx,r8,r9

	push	%rbp			# save required regs
	mov	%rsp,%rbp
	push	%rbx
	push	%rsi
	push	%rdi
	push	%rbp

	call	glfwInit		# no args

	mov	$139266,%rdi		# VERSION_MAJOR
	mov	$3,%rsi			# set to 3
	call	glfwWindowHint

	mov	$139267,%rdi		# VERSION_MINOR
	mov	$3,%rsi			# set to 3
	call	glfwWindowHint

	mov	$139272,%rdi		# OPENGL_PROFILE
	mov	$204801,%rsi		# CORE_PROFILE
	call	glfwWindowHint

	mov	$800,%rdi		# width
	mov	$600,%rsi		# height
	leaq	title(%rip),%rdx	# title
	xor	%rcx,%rcx		# null
	xor	%r8,%r8			# null
	call	glfwCreateWindow

	mov	%rax,%rbx		# save window pointer
	mov	%rax,%rdi		# window pointer
	call	glfwMakeContextCurrent
	
.forever:
	movss	.color_0_2(%rip),%xmm0
	movss	.color_0_3(%rip),%xmm1
	movaps	%xmm1,%xmm2
	movss	.color_1_0(%rip),%xmm3
	call	glClearColor

	mov	$16384,%rdi		# GL_COLOR_BUFFER_BIT
	call	glClear

	mov	%rbx,%rdi		# window pointer
	call	glfwSwapBuffers

	call	glfwPollEvents		# no args

	mov	%rbx,%rdi
	call	glfwWindowShouldClose	# window pointer

	test	%rax,%rax		# if 0
	je	.forever

	call	glfwTerminate

	mov	$0,%rax			# return 0

	pop	%rbp			# restore callee-saved
	pop	%rdi
	pop	%rsi
	pop	%rbx
	mov	%rbp,%rsp
	pop	%rbp
	ret

# Floats
	.align 4
.color_1_0:
	.long	1065353216

	.align 4
.color_0_3:
	.long	1050253722

	.align 4
.color_0_2:
	.long	1045220557

