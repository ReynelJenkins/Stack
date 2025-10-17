; Проверка меток и всех типов переходов

; Считаем 3 + 2 * 4 и выведем результат

PUSH 3
PUSH 2
PUSH 4
MUL
ADD
OUT             ; => ожидаем 11

; Теперь условные переходы
PUSH 5
PUSH 5
JE :equal       ; должно перейти, потому что 5 == 5

PUSH 999        ; не должно выполниться
OUT
JMP :end

:equal
PUSH 42
OUT             ; => ожидаем 42

; Проверка неравенства
PUSH 7
PUSH 9
JNE :notequal   ; должно перейти

PUSH 999        ; не должно выполниться
OUT
JMP :end

:notequal
PUSH 1
OUT             ; => ожидаем 1

; Проверим JB, JBE, JA, JAE
PUSH 2
PUSH 3
JB :less        ; 2 < 3 → переходим
PUSH 999
OUT
JMP :end

:less
PUSH 10
OUT             ; => ожидаем 10

PUSH 3
PUSH 3
JBE :lesseq     ; 3 ≤ 3 → переходим
JMP :end

:lesseq
PUSH 20
OUT             ; => ожидаем 20

PUSH 5
PUSH 3
JA :greater     ; 5 > 3 → переходим
JMP :end

:greater
PUSH 30
OUT             ; => ожидаем 30

PUSH 5
PUSH 5
JAE :greatereq  ; 5 ≥ 5 → переходим
JMP :end

:greatereq
PUSH 40
OUT             ; => ожидаем 40

:end
HLT
