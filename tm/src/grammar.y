%{
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../util/set.h"
#include "transition.h"
#include "tape.h"
#include "turingmachine.h"

extern int yylex();
extern int yyparse();
extern FILE *yyin;

extern Turingmachine tm;


void yyerror(const char *s);
%}


%code requires {
#include "grammar.tab.h"
#include "../../util/set.h"
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
      String_Set states = $states;
      String_Set ioalph = $ioalphabet;
      String_Set tapealph = $tapealphabet;
      Transition_Table delta = $delta;
      char *start_state = $start;
      char *end_state   = $end;
      char *blank_sym   = $blank;
      /* consistency checks: */
      start_state = value_in_set(&states, start_state);
      if(!start_state) {
        fprintf(stderr, "CONSISTENCY ERROR: start state is not a valid state\n");
        YYABORT;
      }
      end_state = value_in_set(&states, end_state);
      if(!end_state) {
        fprintf(stderr, "CONSISTENCY ERROR: end state is not a valid state\n");
        YYABORT;
      }
      blank_sym = value_in_set(&tapealph, blank_sym);
      if(!blank_sym) {
        fprintf(stderr, "CONSISTENCY ERROR: blank symbol not in tape alphabet\n");
        YYABORT;
      }
      if(value_in_set(&ioalph, blank_sym)) {
        fprintf(stderr, "CONSISTENCY ERROR: blank symbol in i/o alphabet\n");
        YYABORT;
      }
      if(!is_subset(&ioalph, &tapealph)) {
        fprintf(stderr, "CONSISTENCY ERROR: i/o alphabet not contained in tape alphabet\n");
        YYABORT;
      }
      if(!consistent_table(&delta, &states, &tapealph, start_state, end_state)) {
        fprintf(stderr, "CONSISTENCY ERROR: states / tapealphabet do not fit for\
          given transitions\n");
        YYABORT;
      }
      /* "output" */
      tm.states = malloc(sizeof(*(tm.states)));
      *(tm.states) = states;
      tm.ioalphabet = malloc(sizeof(*(tm.ioalphabet)));
      *(tm.ioalphabet) = ioalph;
      tm.tapealphabet = malloc(sizeof(*(tm.tapealphabet)));
      *(tm.tapealphabet) = tapealph;
      tm.delta = malloc(sizeof(*(tm.delta)));
      *(tm.delta) = delta;
      tm.current_state = start_state;
      tm.end_state = end_state;
      tm.tape = malloc(sizeof(*(tm.tape)));
      *(tm.tape) = new_tape(blank_sym);
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

