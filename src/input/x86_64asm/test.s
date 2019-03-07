	.text	
title: .string "Keyboard Demo - OpenGL"
	.global main
main:
	# rdi,rsi,rdx,rcx,r8,r9

	push	%rbp
	mov		%rsp,%rbp

	push	%rbx
	push	%rsi
	push	%rdi
	push	%rbp

	call	glfwInit			# no args

	mov		$139266,%rdi		# VERSION_MAJOR
	mov		$3,%rsi
	call	glfwWindowHint

	mov		$139267,%rdi		# VERSION_MINOR
	mov		$3,%rsi
	call	glfwWindowHint

	mov		$139272,%rdi		# OPENGL_PROFILE
	mov		$204801,%rsi		# CORE_PROFILE
	call	glfwWindowHint

	mov		$800,%rdi			# WIDTH
	mov		$600,%rsi			# HEIGHT
	leaq	title(%rip),%rdx	# "Keyboard Demo - OpenGL"
	xor		%rcx,%rcx			# NULL
	xor		%r8,%r8				# NULL
	call	glfwCreateWindow

	mov		%rax,%rbx			# save window pointer
	mov		%rax,%rdi			# window pointer
	call	glfwMakeContextCurrent
	
.forever:						# todo: glfwWindowShouldClose()
	movss	.color_0_2(%rip),%xmm0
	movss	.color_0_3(%rip),%xmm1
	movaps	%xmm1,%xmm2
	movss	.color_1_0(%rip),%xmm3
	call	glClearColor

	mov		$16384,%rdi			# GL_COLOR_BUFFER_BIT
	call	glClear

	mov		%rbx,%rdi			# window pointer
	call	glfwSwapBuffers
	jmp		.forever


	mov		$0,%rax				# return 0

	pop		%rbx
	pop		%rsi
	pop		%rdi
	pop		%rbp

	mov		%rbp,%rsp
	pop		%rbp
	ret	

	.align 4
.color_1_0:
	.long	1065353216

	.align 4
.color_0_3:
	.long	1050253722

	.align 4
.color_0_2:
	.long	1045220557


.vertexShaderSrc: .asciz "#version 330 core\nlayout (location = 0) in vec3 aPos;\nvoid main()\n{\n   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n}\0"
.fragmentShaderSrc: .asciz "#version 330 core\nout vec4 FragColor;\nvoid main()\n{\n   FragColor = vec4(%ff, %ff, %ff, 1.0f);\n}\n\0"
