#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turingmachine.h"
#include "transition.h"
#include "set.h"

Turingmachine tm;

void print_result()
{
  while(value_in_set(tm.ioalphabet, get_content(tm.tape)) != NULL) {
    printf("%s ", get_content(tm.tape));
    right(tm.tape);
  }
  printf("\n");
}

void run_tm()
{
  while(strcmp(tm.current_state, tm.end_state) != 0) {
    Action action = get_action(tm.delta, tm.current_state, get_content(tm.tape));
    /* check for neutral move */
    if(strcmp(tm.current_state, action.p) == 0 && action.X == NEUTRAL &&
        strcmp(get_content(tm.tape), action.b) == 0) {
      fprintf(stderr, "RUNTIME ERROR: Turingmachine is stuck in trivial movement");
      break;
    }
    set_content(tm.tape, action.b);
    tm.current_state = action.p;
    switch(action.X) {
      case LEFT: left(tm.tape);
                 break;
      case RIGHT: right(tm.tape);
                  break;
      case NEUTRAL: /* stay */
                  break;
      default: break;
    }
  }
}
