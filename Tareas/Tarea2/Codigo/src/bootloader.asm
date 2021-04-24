org 0x7C00

; Precompiler defined value for easy changing
%define SECTOR_AMOUNT 0x10  

; Part of the BIOS
jmp short start
nop
                              
OEMLabel		      db "Boot"	        ; Disk label
BytesPerSector		dw 512		        ; Bytes per sector
SectorsPerCluster	db 1		          ; Sectors per cluster
ReservedForBoot		dw 1		          ; Reserved sectors for boot record
NumberOfFats		  db 2		          ; Number of copies of the FAT
RootDirEntries		dw 224		        ; Number of entries in root dir
LogicalSectors		dw 2880		        ; Number of logical sectors
MediumByte		    db 0F0h		        ; Medium descriptor byte
SectorsPerFat		  dw 9		          ; Sectors per FAT
SectorsPerTrack		dw 18		          ; Sectors per track (36/cylinder)
Sides			        dw 2		          ; Number of sides/heads
HiddenSectors		  dd 0		          ; Number of hidden sectors
LargeSectors		  dd 0		          ; Number of LBA sectors
DriveNo			      dw 0		          ; Drive No: 0
Signature		      db 41		          ; Drive signature: 41 for floppy
VolumeID		      dd 00000000h	    ; Volume ID: any number
VolumeLabel		    db "Boots  "      ; Volume Label: any 11 chars
FileSystem		    db "FAT12   "	    ; File system type


start: 

  mov ah, 0   
	mov al, 13h ; 320x200
	int  0x10

  mov si, welcomeMsg  
  call printString
 

tmp:
  call keyboardINTListener  

  startGame:
    ;Initialize Registers
    cli                   ; disable interrupts
    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov sp, 0x6ef0        
    sti                   ; enable interrupts

    mov ah, 0
    int 0x13              ; 0x13 ah=0 dl = drive number
    jc error

    ; Load the kernel into RAM                        
    mov bx, 0x8000        ; Direction where the kernel starts
    mov al, SECTOR_AMOUNT ; Number of sectors to read from kernel
    mov ch, 0             ; Disk configuration
    mov dh, 0             
    mov cl, 2             
    mov ah, 2             
    int 0x13   		      
    jc error
    jmp 0x8000

keyboardINTListener: ;interrupt handler for keyboard events
  pusha 
  mov bx, 0
  inc bx
  in al,0x60 
  btr ax, 7 
  jnc .keyDown
  dec bx 

  .keyDown:
    cmp al,0x1c ;enter
    jne .check
    call startGame
    
  .check:
    mov al, 20h 
    out 20h, al 

  popa 
  jmp keyboardINTListener	


printCharacter:

	mov bh, 0x00 
	mov bl, 0x0F
	mov ah, 0x0E 
	int 0x10 
	ret	

printString:
	pusha 

	.loop:
		lodsb 
		test al, al
		jz .end
		call printCharacter ;print character in al    
	  jmp .loop ;print next character

	.end:
	  popa ;restore registers to original state

	ret

error:                  ;error message
  mov si, errorMsg
  mov bh, 0x00          ;page 0
  mov bl, 0x07          ;text attribute
  mov ah, 0x0E          ;tells BIOS to print char

.part:
  lodsb
  sub al, 0
  jz end
  int 0x10              ;interrupt
  jmp .part

end:
  jmp $

errorMsg db "Failed to load...", 0
welcomeMsg db "Welcome to Finding Treasure booteable", 0xA, 0xD, "Press enter to play", 0xA, 0xD, 0
times 510 - ($ - $$) db 0

; Begin MBR Signature
db 0x55 ; byte 511 = 0x55
db 0xAA ; byte 512 = 0xAA