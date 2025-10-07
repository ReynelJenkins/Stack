   PUSH 10
        POPREG AX
PUSHREG AX
PUSHREG AX
MUL
OUT
      PUSHREG AX
                PUSH 1
SUB
POPREG AX
PUSHREG AX
PUSH 1
JAE 4
HLT
; TODO: Решение квадратного уравнения с разбором всех случаев если ур е сводится к линейному то печатаем -1
; 2-ая часть решение вообще всех ур й
