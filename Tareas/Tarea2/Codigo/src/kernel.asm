org 0x8000
bits 16 

%define entityArraySize 256

mov ax, 10
mov [enemyPosX], ax

mov ax, -40
mov [enemyPosZ], ax



init: 
	; Start graphic mode
	call initGraphics

	; Start interrupt handler
	call registerInterruptHandlers

	; Init map
	call initMap

; Main game loop
gameLoop:
	call resetBuffer 					; reset screen 
	
	
	mov di, entityArray
	add di, 2 

	.nextEntity:
		cmp [di], word 0
		je .skip

	pusha
	mov cx, [player + 2] 		; player x to draw relative
	mov dx, [player + 4] 		; player z to draw relative
	mov di, [di]
	call drawEntity
	popa

	.skip:
		add di, 2
		cmp di, entityArray + ((entityArraySize - 1) * 2) 
		jl .nextEntity

	call drawMap
	
	mov si, [player]   							; get animation
	mov ax, [player + 6]  					; get index within animation
	xor dx,dx
	div word [si + 2]    		
	mov ax, dx
	xor dx, dx
	div word [si]     
	add ax, ax         
	
	add si, 4          
	add si, ax		   								; add the offset to the frame
	mov si, [si]       							; set the image parameter to the image referenced in the frame
	
	mov ax, (80 / 2) - (9 / 2) - 1      ; center player image
	mov bx, (50 / 2) - (12 / 2) - 1     ; center player image
	call drawImage

	mov ax, [enemyPosX]    ;center player image
	mov bx, [enemyPosZ]    ;center player image
	sub ax, 2
	sub bx, 2
	mov si, enemyImage
	call drawImage
	
	call copyBufferOver 						; draw frame to screen
	
	call gameControls 							; handle control logic
 	
	call synchronize 								; synchronize emulator and real application through delaying
	
jmp gameLoop

jmp $

; di = entity cx,dx = xpos,zpos
; Draw the game entities
drawEntity:
	push dx
	inc word [di + 6]
	mov ax, [di + 6] 								; get index within animation
	mov si, [di]
	xor dx,dx
	div word [si + 2]    
	mov ax, dx
	xor dx, dx
	div word [si]      
	add ax, ax         
	
	add si, 4          
	add si, ax		   
	mov si, [si]       
	pop dx
		
	mov ax, word [di + 2] 							; get entity x
	sub ax, cx          								; subtract the position of the player from the x position
	add ax, (80 / 2) - (9 / 2) - 1  		; relative to screen image drawing code for x position
	mov bx, word [di + 4] 							; get entity y
	sub bx, dx          								; subtract the position of the player from the z position
	add bx, (50 / 2) - (12 / 2) - 1 		; relative to screen image drawing code for z position
	call drawImage      								; draw image to buffer
	ret

; di = entity, cx = new_xpos, dx = new_zpos, bp = new animation
; Check for playet collisions
checkForCollision:
	pusha                       
	mov si, entityArray  

	.whileLoop:
		mov bx, word [si]   					; read entityArray entry
		test bx, bx         					; end of array
		jz .whileSkip
		cmp bx, di          					; next entity to not collide with it self
		jz .whileSkip
	
	mov ax, word [bx + 2] 
	sub ax, 8           						; subtract 8 because of hitbox
	cmp ax, cx 											
	jg .whileSkip
		
	mov ax, word [bx + 2] 					; ax = entity x
	add ax, 8           						; add 8 because of hitbox
	cmp ax, cx 
	jle .whileSkip

	mov ax, word [bx + 4] 					; ax = entity z
	sub ax, 10          						; subtract 10 because of hitbox
	cmp ax, dx
	jg .whileSkip
		
	mov ax, word [bx + 4] 					; ax = entity z
	add ax, 9           						; subtract 9 because of hitbox
	cmp ax, dx 
	jle .whileSkip	
	
	
	mov word [si], 0
	inc word [coinFound]
		
	jmp .noMapCollision

	.whileSkip:
		add si, 2           					; set si to the next entry in the entityArray
		cmp si, entityArray + ((entityArraySize - 1) * 2)
		jl .whileLoop
		.whileEnd

	pusha
	mov si, cx
	mov bx, dx
	call collideMap
	popa
	jnc .noMapCollision

	mov cx, [di + 2]         				; set new x pos to current x pos => no movement
	mov dx, [di + 4]         				; set new z pos to current z pos => no movement

	.noMapCollision:
		mov byte [canWalk], 1
		mov word [di],  bp  					; update the animation in use
		mov word [di + 2], cx  				; update x pos
		mov word [di + 4], dx  				; update y pos
		popa                 					; reload old register state
		
	ret

pauseGameFunc:
	in al, 0x60
	cmp al, 0x26
	je pauseGame
	jmp pauseGameFunc

reset:
	jmp init	

canWalk db 0

gameControls:
	mov al, byte [pressL]
	cmp al, 0
	jz pauseGame
	jmp pauseGameFunc

	pauseGame:

	mov al, byte [pressR]
	cmp al, 1
	jz reset

	mov byte [canWalk], 0
	mov di, player 									; select the player as the main entity for "checkForCollision"
	mov al, byte [pressLeftArrow]
	add al, byte [pressRightArrow]
	cmp al, 0
	jz .nokeyad

	mov cx, word [playerPosX] 		  ; set cx to player x
	mov dx, word [playerPosZ] 		  ; set dx to player z
	mov bp, [player]             	  ; set bp to current animation
	cmp byte [pressRightArrow], 1   ; move x+1 if 'd' is pressed 
	jne .nd
	inc cx
	mov bp, playerImgRight

	pusha
	mov cx, word [enemyPosX] 				;set cx to enemy x
	inc cx
	mov [enemyPosX], cx
	popa

	.nd:
		cmp byte [pressLeftArrow], 1 	; move x-1 if 'a' is pressed
		jne .na
		dec cx
		mov bp, playerImgLeft

		pusha
		mov cx, word [enemyPosX] 	 ; set cx to enemy x
		dec cx
		mov [enemyPosX], cx
		popa

	.na:
		call checkForCollision 				; check if player would collide on new position

	.nokeyad:
		mov al, byte [pressUpArrow]
		add al, byte [pressDownArrow]
		cmp al, 0
		jz .nokeyws

	mov cx, word [playerPosX] 			; set cx to player x
	mov dx, word [playerPosZ] 			; set dx to player z
	mov bp, [player]           			; set bp to current animation
	cmp byte [pressUpArrow], 1 			; try to move z-1 if 'w' is pressed
	jne .nw
	dec dx
	mov bp, playerImgBack

	pusha
	mov cx, word [enemyPosZ] 			; set cx to enemy z
	inc cx
	mov [enemyPosZ], cx
	popa

	.nw:
		cmp byte [pressDownArrow], 1 	; try to move z+1 if 's' is pressed
		jne .ns
		inc dx
		mov bp, playerImgFront

		pusha
		mov cx, word [enemyPosZ] 		 ; set cx to enemy z
		dec cx
		mov [enemyPosZ], cx
		popa

	.ns:
		call checkForCollision 				; check if player would collide on new position

	.nokeyws:
		cmp byte [canWalk], 0
		jnz .noCollision
		mov word [player + 6], 0 	    ; reset animation counter
		ret

	.noCollision:
		inc word [player + 6]  				; update animation if moving
		ret
	

registerInterruptHandlers:
	mov [0x0024], dword keyboardINTListener ; implements keyboardListener
	ret
	
; Keyboard event based code
pressLeftArrow db 0
pressRightArrow db 0
pressUpArrow db 0
pressDownArrow db 0
pressSpacebar db 0
pressL db 0
pressR db 0

; Interrupt handler for keyboard events
keyboardINTListener: 
	pusha	
	xor bx, bx 												    ; bx = 0: signify key down event
	inc bx
	in al, 0x60 											    ; get input to AX, 0x60 = ps/2 first port for keyboard
	btr ax, 7 											
	jnc .keyDown
	dec bx 														; bx = 1: key up event

	.keyDown:
		cmp al, 0x4b ;a
		jne .check1         
		mov byte [cs:pressLeftArrow], bl ; use cs overwrite because we don't know where the data segment is
	.check1:
		cmp al, 0x4d 										 ; left arrow
		jne .check2
		mov byte [cs:pressRightArrow], bl

	.check2:
		cmp al, 0x48 										 ; right arrow
		jne .check3
		mov byte [cs:pressUpArrow], bl

	.check3:
		cmp al, 0x50 									   ; up arrow
		jne .check4
		mov byte [cs:pressDownArrow], bl

	.check4:
		cmp al, 0x39 										 ; down arrow
		jne .check5
		mov byte [cs:pressSpacebar], bl

	.check5:
		cmp al, 0x26 										 ; l
		jne .check6
		mov byte [cs:pressL], bl

	.check6:
		cmp al, 0x13     								 ; r
		jne .check7
		mov byte [cs:pressR], bl

	.check7:
		mov al, 20h 
		out 20h, al 										; acknowledge the interrupt so further interrupts can be handled again 
	popa 															; resume state to not modify something by accident
	iret 															; return from an interrupt routine	

; Synchronize the game, applying a delay
synchronize:
	pusha
		mov si, 20 											; si = time in ms
		mov dx, si
		mov cx, si
		shr cx, 6
		shl dx, 10
		mov ah, 86h
		int 15h 												; cx,dx sleep time in microseconds - cx = high word, dx = low word
	popa
	ret


; cx, dx = xpos, zpos, si = animation
; eax == 0 success, else failed
addEntity:
	pusha
	mov bx, cx
	mov di, entityArray
	xor ax, ax
	mov cx, (entityArraySize - 1)
	repne scasw                 			; iterate through entity array until empty stop is found
	sub di, 2
	test ecx, ecx               			; abort here if at the end of the the entity array
	je .failed
	sub cx, (entityArraySize - 1) 		; calculate index within the array by using the amount of iterated entires
	neg cx
	shl cx, 3
	add cx, entityArrayMem
	mov [di], cx
	mov di, cx
	mov [di], si
	mov [di + 2], bx 									; set x position of the entity
	mov [di + 4], dx 									; set y position of the entity
	xor bx, dx     
	mov [di + 6], bx 									; set animation state
	popa
	xor eax, eax   										; return 0 if successfully added
	ret

	.failed:
		popa
		xor eax, eax
		inc eax       									; return 1 if failed to find a place for the entity
		ret

; di = entity cx,dx = xpos,zpos
drawBlock:
	mov ax, word [player + 2]
	sub ax, cx
	imul ax, ax
	mov bx, word [player + 4]
	sub bx, dx
	imul bx, bx
	add ax, bx
	cmp ax, 3000 
	jge .skip

	mov ax, cx
	mov bx, dx
	sub ax, word [player + 2]   			;subtract the position of the player from the x position
	add ax, (80 / 2) - (9 / 2) - 1    ;relative to screen image drawing code for x position
	sub bx, word [player + 4]   			;subtract the position of the player from the z position
	add bx, (50 / 2) - (12 / 2) - 1   ;relative to screen image drawing code for z position
	call drawImage            				;draw image to buffer

	.skip:
		clc
		
	ret
	
; Set the position of the player to x=cx, z=dx
setSpawn:
	mov word [player + 2], cx ; set player x
	mov word [player + 4], dx ; set player z
	add word [player + 4], 3  ; offset player z
	clc
	ret
	

; Draw the map
drawMap:
	mov si, boxImg0
	mov bp, drawBlock
	mov ah, '0'
	call iterateMap 				; iterate the map and add a box at every '0' on the map
	
	mov si, tileImg0
	mov bp, drawBlock
	mov ah, ' '
	call iterateMap 				; iterate the map and add a tile at every ' ' on the map

	ret

; Spawn the enemies and eagle, add set the spawn position of the player
initMap:
	mov si, bomb
	mov bp, addEntity
	mov ah, 'B'
	call iterateMap  				; iterate the map and add at every 'B' on the map


	mov si, destructableWall
	mov bp, addEntity
	mov ah, 'W'
	call iterateMap  				; iterate the map and add an eagle at every 'W' on the map

	mov si, enemy
	mov bp, addEntity
	mov ah, 'E'
	call iterateMap  				; iterate the map and add at every 'E' on the map

	call spawnPlayer 				; set spawn for player
	ret


; si = player X, bx = player Y
collideMap:
	mov bp, blockCollison
	mov ah, '0'
	call iterateMap 				; iterate the map and check for a collision with a '0'

	ret

; Set the spawn of the player to the field 'P'
spawnPlayer:
	mov bp, setSpawn
	mov ah, 'P'
	call iterateMap 				; iterate the map and set the player position to the last 'P' found on the map
	ret

%define tileWidth      8
%define ASCIImapWidth  64
%define ASCIImapHeight 64

; bp = function to call, ah = search for, si = parameter for bp function
iterateMap:
	mov di, ASCIImap
	mov cx, 0x0 													; map start x
	mov dx, 0x0 													; map start y

	.next:
		mov al, [di]
		test al, al
		je .stop    												; stop when null terminator found
		cmp al, ah
		jne .skip   												; skip if the character is not the one this iteration is searching for
		push ax     												; save the content of ax
		call bp     												; call the specified function of this iteration
		pop ax
		jc .term

	.skip:
		inc di                           		  ; point to the next character
		add cx, tileWidth                		  ; increase x pixel position
		cmp cx, ASCIImapWidth * tileWidth  		; check if x position is at the end of the line
		jl .next

	sub dx, tileWidth                     ; decrease y pixel position
	xor cx, cx                            ; reset x position
	jmp .next

	.stop:
		clc

	.term:
		ret

; si = player x, bx = player z, cx = block x, dx = block z
blockCollison:
	push cx
	push dx
	sub cx, 8    						; subtract 8 because of hitbox
	cmp cx, si
	jg .skip

	add cx, 8 + 8           ; add 8 because of hitbox
	cmp cx, si
	jle .skip

	sub dx, 10           		; subtract 10 because of hitbox
	cmp dx, bx
	jg .skip

	add dx, 9 + 10         	; subtract 9 because of hitbox
	cmp dx, bx
	jle .skip

	stc
	jmp .end

	.skip:
		clc

	.end:
		pop dx
		pop cx
		ret

%include "./src/buffer.asm"

coinFound dw 0

; Entity array
entityArray:
	dw player
	resw entityArraySize

; Player structure
player:
	playerAnim  dw playerImgFront 		 ; pointer to animation
	playerPosX  dw 0x32              	 ; position of player (x)
	playerPosZ  dw 0x32                ; position of player (z)
	playerAnimC dw 0               		 ; animation counter

enemyStruct:
	enemyPosX  dw 0x10            ; position of enemy (x)
	enemyPosZ  dw 0x10            ; position of enemy (z)

; Other entity structures:
entityArrayMem:
	resw entityArraySize * 4

; Animation structure
playerImgFront:
	dw 5
	dw 20
	dw playerImgFront0
	
	dw playerImgFront0
	dw playerImgFront0
	dw playerImgFront0
	dw 0

playerImgBack:
  	dw 5
	dw 20
	dw playerImgBack0
	dw playerImgBack0
	dw playerImgBack0
	dw playerImgBack0
	dw 0

playerImgRight:
  	dw 5
	dw 20
	dw playerImgRight0
	dw playerImgRight0
	dw playerImgRight0
	dw playerImgRight0
	dw 0

playerImgLeft:
	dw 5
	dw 20
	dw playerImgLeft0
	dw playerImgLeft0
	dw playerImgLeft0
	dw playerImgLeft0
	dw 0

boxImg:
	dw 1            ; time per frames
	dw 1            ; time of animation
	dw boxImg0      ; frames
	dw 0            ; zero end frame

bomb:
	dw 1            ; time per frames
	dw 1            ; time of animation
	dw bombImg      ; frames
	dw 0            ; zero end frame

destructableWall:
	dw 1
	dw 1
	dw wall0
	dw 0

enemy:
	dw 1
	dw 1
	dw enemyImage
	dw 0

playerImgFront0 	incbin "img/player_front_0.bin"
playerImgBack0  	incbin "img/player_back_0.bin"
playerImgRight0 	incbin "img/player_right_0.bin"
playerImgLeft0  	incbin "img/player_left_0.bin"



bombImg          incbin "img/bombv2.bin"

boxImg0            incbin "img/box.bin"
wall0             incbin "img/ice.bin"
tileImg0          incbin "img/tile.bin"
enemyImage		  incbin "img/enemy.bin"

ASCIImap          incbin "img/mapTemplate.bin"
;db 0



%assign usedMemory ($ - $$)
%assign usableMemory (512 * 32)
%warning [usedMemory/usableMemory] Bytes used


times (512 * 32) - ($ - $$) db 0 