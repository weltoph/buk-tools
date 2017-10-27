%{
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

extern int yylex();
extern int yyparse();
extern FILE *yyin;

void yyerror(const char *s);
%}


%code requires {
#include "grammar.tab.h"
#include "set.h"
#include "transition.h"
}

%code {
}

%union {
  char identifier[30];
  MOVEMENT_DIRECTION movement_direction;
  String_Set identifier_set;
  Transition trans;
  Transition_Table trans_table;
}

%token <identifier> IDENTIFIER
%token <movement_direction> MOVEMENT
%token SETLEFT
%token SETRIGHT
%token TUPELLEFT
%token TUPELRIGHT
%token MAPPING
%token COMMA
%token DELIMITER

%type <identifier_set> identifier_set
%type <identifier_set> identifier_list
%type <trans> trans
%type <trans_table> transition_set
%type <trans_table> transition_list


%%

spec:
    identifier_set[states] DELIMITER identifier_set[ioalphabet] DELIMITER 
    identifier_set[tapealphabet] DELIMITER IDENTIFIER[blank] DELIMITER 
    IDENTIFIER[start] DELIMITER IDENTIFIER[end] DELIMITER transition_set[delta]{
      char *states = set_repr(&$states);
      char *ioalphabet = set_repr(&$ioalphabet);
      char *tapealphabet = set_repr(&$tapealphabet);
      Transition_Table delta = $delta;
      free_table(&delta);
      fprintf(stderr, "parsed spec with:\n");
      fprintf(stderr, "states: %s\n", states);
      fprintf(stderr, "ioalphabet: %s\n", ioalphabet);
      fprintf(stderr, "tapealphabet: %s\n", tapealphabet);
      fprintf(stderr, "blank-symbol: %s\n", $blank);
      fprintf(stderr, "start-state: %s\n", $start);
      fprintf(stderr, "end-state: %s\n", $end);
      free(states);
      free(ioalphabet);
      free(tapealphabet);
      free_set(&$states);
      free_set(&$ioalphabet);
      free_set(&$tapealphabet);
    }
    ;

identifier_set:
   SETLEFT identifier_list SETRIGHT {
      $$ = $2;
   }
   ;

identifier_list:
               IDENTIFIER {
                  String_Set set = new_set();
                  add_value(&set, $1);
                  $$ = set;
               }
               | identifier_list COMMA IDENTIFIER {
                  add_value(&$1, $3);
                  $$ = $1;
               }
               ;

trans:
     TUPELLEFT IDENTIFIER[q] COMMA IDENTIFIER[a] TUPELRIGHT MAPPING
     TUPELLEFT IDENTIFIER[p] COMMA IDENTIFIER[b] COMMA MOVEMENT[X] TUPELRIGHT{
        Action rhs = new_action($p, $b, $X);
        $$ = new_trans($q, $a, rhs);
     }
     ;

transition_set:
              SETLEFT transition_list SETRIGHT {
                $$ = $2;
              } 
              ;

transition_list:
               trans {
                  Transition_Table trans_table = new_trans_table();
                  Transition t = $1;
                  add_trans(&trans_table, t);
                  free(t.q);
                  free(t.a);
                  $$ = trans_table;
               }
               | transition_list COMMA trans {
                  Transition_Table trans_table = $1;
                  Transition t = $3;
                  add_trans(&trans_table, t);
                  free(t.q);
                  free(t.a);
                  $$ = trans_table;
               }
               ;
%%

void yyerror(const char *s)
{
  printf("\nAn error occurred: %s\n", s);
}

