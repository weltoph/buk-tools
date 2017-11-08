/* Scanner for the while language */
%{
#include <stdlib.h> /* atoi for identifier index */
#include <string.h>
#include <stdint.h>
#include "while.tab.h"
%}

%option noinput
%option nounput
%option noyywrap

INDEX 0|[1-9][0-9]*
WHITESPACE [ \t]*

%%

[ \n\t]

x{INDEX} {
    const uint8_t index = atoi(yytext + 1);
    yylval.index = index;
    return IDENTIFIER;
  }

0|1 {
    yylval.value = atoi(yytext);
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

- {
    return MINUS;
   }

\/={WHITESPACE}0 {
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
