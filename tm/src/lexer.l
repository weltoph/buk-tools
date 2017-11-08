/* Scanner for a Turingmachine specification language */
%{
#include <stdlib.h> /* atoi for identifier index */
#include <stdint.h>
#include <stdio.h>
#include "grammar.tab.h"
#include "transition.h"
%}

%option noinput
%option nounput
%option noyywrap

IDENTIFIER [a-z0-9]

%%

[ \n\t]

{IDENTIFIER}{1,28} {
    strcpy(yylval.identifier, yytext);
    return IDENTIFIER;
  }

L|R|N {
    const char dir = yytext[0];
    switch(dir) {
      case 'L':
        yylval.movement_direction = LEFT;
        break;
      case 'N':
        yylval.movement_direction = NEUTRAL;
        break;
      case 'R':
        yylval.movement_direction = RIGHT;
        break;
      default:
        break;
    }
    return MOVEMENT;
  }

\{ {
    return SETLEFT;
  }

\} {
    return SETRIGHT;
  }

\( {
    return TUPELLEFT;
  }

\) {
    return TUPELRIGHT;
  }

-> {
    return MAPPING;
  }

, {
    return COMMA;
  }

; {
    return DELIMITER;
  }

%%
