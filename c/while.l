/* Scanner for the while language */
%{
#include <stdlib.h> /* atoi for identifier index */
#include <string.h>
#include <stdint.h>
#include "while.tab.h"
%}


INDEX 0|[1-9][0-9]*

%%

[ \n\t];

x{INDEX} {
    const uint8_t index = atoi(yytext + 1);
    yylval.index = index;
    return IDENTIFIER;
  }

-1 {
    yylval.value = -1;
    return CONSTANT;
  }

0 {
    yylval.value = 0;
    return CONSTANT;
  }

1 {
    yylval.value = 1;
    return CONSTANT;
  }

; {
    return DELIMITER;
  }

:= {
    return ASSIGNMENT;
  }

\+ {
    return PLUS;
  }

\/=0 {
    return NOT_EQUAL_ZERO;
  }

while {
    return WHILE;
  }

do {
    return DO;
  }

end {
    return END;
  }

%%
