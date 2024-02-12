create inputBuffer 80 allot
variable continueFlag

code prepareScreen
    lda #5      ; green
    sta $0286   ; text color
    lda #$17     
    sta $D018   ; set mixed mode 
    jsr $e544   ; clear screen
    lda #$00    ; black  
    sta $d020   ; border 
    sta $d021   ; background
    rts
;code

code restoreScreen
    lda #$f6
    sta $d021 ;background
    lda #$fe  
    sta $d020 ;border
    lda #$e
    sta $0286 ;text color
    jsr $e544 ;clear screen
    lda #$15  ;set uper case 
    sta $d018 ; text mode
	rts
;code

: clearBuffer
    inputBuffer 80 0 fill
;

: main 
    prepareScreen

    
    ."              Calculator " cr
    ." Enter data for calculation using RPN: " cr cr cr 
    
    begin
        clearBuffer
        inputBuffer 80 accept cr  
        
        inputBuffer @ 'q' = if
            -1 continueFlag !
        else
            0 continueFlag !
        then     
    
    continueFlag @ until 
    
    restoreScreen
;

compile main