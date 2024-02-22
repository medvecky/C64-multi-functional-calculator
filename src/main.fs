create inputBuffer 80 allot

variable inputBufferActualLength
variable continueFlag
variable substringStartPosition
variable substringLength
variable replaceIndex


code prepareScreen
    lda #5      ; green
    sta $0286   ; text color
    lda #$17     
    sta $D018   ; set mixed mode 
    jsr $e544   ; clear screen
    dex
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

: debugString ( len -- )
    0 do
       i . inputBuffer i + c@ . inputBuffer i + c@ emit cr 
    loop
;

: stringLenght ( addr - len)
    
    dup substringStartPosition !
    
    80 0 do
        substringStartPosition @ i + c@ 0 = if
            i 1 +   
            leave
        then
    loop
;

: replaceSpaceToZero
    substringStartPosition @
    substringStartPosition @ substringLength @ + 1 + substringStartPosition !
    0 substringLength !
    replaceIndex @ 1 + inputBufferActualLength @ = if
        substringLength @ 1 + substringLength !
    else 
        0 inputBuffer replaceIndex @ + c!
    then  
;

: parseArgs ( addr len - list of substrings )
    0 substringLength ! 
    dup 0> if
        dup inputBufferActualLength !
    
        0 do
            inputBuffer i + c@ 32 = 
            i 1 + inputBufferActualLength @ = 
            or if
                i replaceIndex !
                replaceSpaceToZero
            else
                substringLength @ 1 + substringLength !
            then    
        loop
        cr
    then    
;

: main 
    prepareScreen

    
    ."              Calculator " cr
    ." Enter data for calculation using RPN: " cr cr cr 
    
    begin
        
        clearBuffer
        inputBuffer 80 accept cr
        
        inputBuffer c@ 'q' = if
            -1 continueFlag !
        else
            0 continueFlag !
            inputBuffer substringStartPosition !
            inputBuffer swap parseArgs

            begin
                stringLenght type cr
            depth 1 = until
        then 
        drop

    continueFlag @ until  
    
    restoreScreen
;

compile main