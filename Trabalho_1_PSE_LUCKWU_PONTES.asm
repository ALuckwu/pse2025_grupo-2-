	org		00h
	
	mov 	TMOD, #001h				;configura o modo do timer
	mov 	R7, #000h 				;inicializa sentido horario
	mov		P1, #000h

config:
	jnb		P2.7, start
	sjmp	config	

start:
	mov		a, #000h
	mov		b, #014h	;20 em B
	jb		P2.2, add4
	return_add4:
	jb		P2.1, add2
	return_add2:
	jb		P2.0, add1
	return_add1:
	add		a, #001h
	mul		AB				;multiplica o valor em acc por 2
	mov		R0, a			;o resultado da multiplicação para R0
	
	jb		P2.6, H_start_Y		;sentido horario
	jnb		P2.6, AH_start_Y	;sentido anti-horario
	add4:
		add		a, #004h
		sjmp	return_add4
	add2:
		add		a, #002h
		sjmp	return_add2
	add1:
		add		a, #001h
		sjmp	return_add1

H_start_Y: 
	mov		R7, #000h			;sinaliza horario
	setb	P1.4
	setb	P1.7
	sjmp	timer_counter
H_stop_Y:
	clr		P1.7
	clr		P1.4
	mov		R0, #001h			;CONFIGURA PARA 100ms
	sjmp	timer_counter
H_start_delta:
	setb	P1.5
	setb	P1.7
	sjmp	H_ready

AH_start_Y:
	mov		R7, #001h			;sinaliza anti-horario
	setb	P1.4
	setb	P1.6
	sjmp	timer_counter

AH_stop_Y:
	clr		P1.6
	clr		P1.4
	mov		R0, #001h			;CONFIGURA PARA 100ms
	sjmp	timer_counter

AH_start_delta:
	setb	P1.5
	setb	P1.6
	sjmp	AH_ready

timer50ms:
	mov		TL0, #0B0h
	mov		TH0, #03Ch
	mov		TCON, #010h			;inicializa timer
	jnb		TCON.5, $			;mantém nessa linha até terminar o tempo do timer
	dec		R0				;conta uma passagem pelo timer
	sjmp	timer_counter

timer_counter:
	cjne	R0, #000h, timer50ms	
	mov 	TCON, #000h		;para timer

finish_timer:
	mov		a, R7		;copia R7 para acc para testar condicoes
	jb		P1.7, H_stop_Y
	jb		P1.6, AH_stop_Y
	jb		ACC.1, delay3s_stop
	jnb		ACC.0, H_start_delta
	jb		ACC.0, AH_start_delta

H_ready:
	jb		P2.7, desligar
	jnb		P2.6, inverter
	sjmp	H_ready

AH_ready:
	jb		P2.7, desligar
	jb		P2.6, inverter
	sjmp	AH_ready

desligar:
	clr		P1.7
	clr		P1.6
	setb	P1.5
	setb	P1.4
	mov		a, R7				;gravar em R7 o estado atual 
	setb	ACC.1
	mov		R7, a
	sjmp	delay3s

inverter:
	SJMP	desligar

delay3s:
	mov		R0, #01eh			;Configura R0 para 30d (3s)
	sjmp	timer_counter

delay3s_stop:
	mov		a, R7
	clr		ACC.1
	mov		R7, a
	ljmp	config
