/* Scanner for a Turingmachine specification language */
%{
#include <stdlib.h> /* atoi for identifier index */
#include <stdint.h>
#include <stdio.h>
#include "tm.tab.h"
%}

%option noinput
%option nounput

INDEX 0|[1-9][0-9]*
WHITESPACE [ \t]*

%%

[ \n\t]

q{INDEX} {
    const char *index_text = yytext + 1;
    const uint8_t index = atoi(index_text);
    yylval.index = index;
    return STATE;
  }

L|R|N {
    const char dir = yytext[0];
    switch(dir) {
      case 'L':
        yylval.direction = LEFT;
        break;
      case 'N':
        yylval.direction = NEUTRAL;
        break;
      case 'R':
        yylval.direction = RIGHT;
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
    return SETSEP;
  }

a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z|0|1|2|3|4|5|6|7|8|9 {
    const char content = yytext[0];
    yylval.character = content;
    return TAPECONTENT;
  }

~ {
    return VOIDED;
  }

; {
    return DELIMITER;
  }

%%
