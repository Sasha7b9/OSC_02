PERIPH_BASE      EQU 0x40000000
AHB1PERIPH_BASE  EQU (PERIPH_BASE + 0x00020000)

GPIOD_BASE       EQU (AHB1PERIPH_BASE + 0x0C00)
GPIOD            EQU GPIOD_BASE
GPIOD_BSRR       EQU GPIOD + 24

GPIOA_BASE       EQU (AHB1PERIPH_BASE + 0x0000)
GPIOA            EQU GPIOA_BASE
GPIOA_IDR        EQU GPIOA + 16

GPIO_PIN_0       EQU 0x01
GPIO_PIN_1       EQU 0x02
GPIO_PIN_2       EQU 0x04
GPIO_PIN_3       EQU 0x08
GPIO_PIN_4       EQU 0x10
GPIO_PIN_5       EQU 0x20
GPIO_PIN_4_5     EQU GPIO_PIN_4 + GPIO_PIN_5
GPIO_PIN_6       EQU 0x40
GPIO_PIN_7       EQU 0x80
    
              AREA |.text|, CODE, READONLY
              
              EXPORT CycleSend [WEAK]
CycleSend PROC

    PUSH {R0-R1, R4, lr}
    
;GPIOD->BSRR = GPIO_PIN_5 << 16
    MOV R0, #GPIO_PIN_5 << 16
    LDR R1, =GPIOD_BSRR
    STR R0, [R1]
    
;while(GPIOA->IDR & GPIO_PIN_7)
cycle1
    LDR R4, =GPIOA_IDR
    LDR R4, [R4]
    AND R4, #GPIO_PIN_7
    CMP R4, #0
    BNE  cycle1
    
    POP {R0-R1, R4, pc}
                  
;    IMPORT mode

; mode = Mode::PanelWrite
;    MOVS R1, #2
;    LDR  R3, |mode|
;    STRB R1, [R3]

; GPIOD->MODER &= 0xfffff0ff
;    LDR R1, =GPIOD
;    BIC R2, R1, #0xf00
;    STR R2, [R1]

; GPIOD->MODER |= 0x500
;    ORR R2, R1, #0x500
;    STR R2, [R1]
    
; GPIOD->BSRR = (GPIO_PIN_5 | GPIO_PIN_4)
;    MOVS R3, #GPIO_PIN_4 + GPIO_PIN_5
;    LDR  R1, =GPIOD_BSRR
;    STR  R3, [R1]

              ENDP
              END
