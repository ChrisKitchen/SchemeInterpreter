#include <stdlib.h>
#include <stdio.h>
#include "tokenizer.h"
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include <assert.h>

#ifndef _PARSER
#define _PARSER
Value *Tree;
// Takes a list of tokens from a Racket program, and returns a pointer to a
// parse tree representing that program.



/*
if(curr->c.car->type == CLOSE_TYPE){
  while (tree->c.car->type != OPEN_TYPE)
  {
    subtree = cons(car(tree), subtree);
    tree = cdr(tree);
  }
  assert(tree->c.car->type == OPEN_TYPE);
  tree = cdr(tree);

  cons(subtree, tree);
}

else{
  cons(car(curr), tree);
}



*/





Value *addToParseTree(Value *curr, Value *tree, int depth) {
  if(curr->c.car->type == CLOSE_TYPE){
    Value *subtree;
    subtree = makeNull();
    while (car(tree)->type != OPEN_TYPE)
    {
      if(depth < 0){
        fprintf(stdout,"Syntax error: not enough close parentheses.");
        texit(0);
      }
      subtree = cons(car(tree), subtree);
      tree = cdr(tree);
      
    }
    // assert(tree->c.car->type == OPEN_TYPE);
    if(tree->c.car->type != OPEN_TYPE){
      fprintf(stderr,"ERROR");
      texit(0);
    }
    tree = cdr(tree);

    tree = cons(subtree, tree);
  }

  else{
    tree = cons(car(curr), tree);
  }
  return tree;
}

int setDepth(int depth, Value *curr){
  if (curr->type == OPEN_TYPE) {
    depth++; 
  } else if(curr->type == CLOSE_TYPE){
    depth--;
  }
  return depth;
}

//a b (c d) (e (f g)) h

Value *parse(Value *tokens) {

    Tree = makeNull();
    int depth = 0;

    Value *current = tokens;
    // assert(current != NULL && "Error (parse): null pointer");
    // if(current == NULL){
    //   fprintf(stderr,"ERROR");
    //   texit(0);
    // }
    while (current->type != NULL_TYPE) {
        Value *token = current;
        depth = setDepth(depth, car(token));
        Tree = addToParseTree(token, Tree, depth);
        current = cdr(current);
    }
    // assert(depth == 0);
    if(depth != 0){
      fprintf(stdout,"Syntax error: not enough close parentheses.");
      texit(0);
    }
    Tree = reverse(Tree);
    return(Tree);
}


// Prints the tree to the screen in a readable fashion. It should look just like
// Racket code; use parentheses to indicate subtrees.
void printTreeH(Value *tree){
  printf(" (");//printing ( ) for each call at beginning/end respectively
  while (tree->type != NULL_TYPE){  //if current->type == pointer recursively call self on sub-trees. 
  if(car(tree)->type == CONS_TYPE){
      printTreeH(car(tree));
  }else{
    printf(" ");
    switch(car(tree)->type){
      case STR_TYPE  :
      fprintf(stdout,"%s\"",car(tree)->s);
      break; 
      case INT_TYPE  :
      fprintf(stdout,"%i",car(tree)->i);
      break; 
      case DOUBLE_TYPE  :
      fprintf(stdout,"%f",car(tree)->d);
      break; 
      case SYMBOL_TYPE  :
      fprintf(stdout,"%s",car(tree)->s);
      break; 
      case OPEN_TYPE  :
      fprintf(stdout,"(:open\n");
      break; 
      case CLOSE_TYPE  :
      fprintf(stdout,"):close\n");
      break; 
      case PTR_TYPE  :
      fprintf(stdout,"POINTER\n");
      break; 
      case BOOL_TYPE  :
      if (car(tree)->i == 0){
        fprintf(stdout,"#f");
      } else {
        fprintf(stdout,"#t");
      }
      break; 
      case DOT_TYPE  :
      fprintf(stdout,".");
      break; 
      case NULL_TYPE  :
      fprintf(stdout,"()");
      break; 
      case SINGLEQUOTE_TYPE  :
      fprintf(stdout," \" ");
      break; 
      case OPENBRACKET_TYPE  :
      fprintf(stdout,"{");
      break; 
      case CONS_TYPE  :
      fprintf(stdout,"CONS\n");
      break; 
      case CLOSEBRACKET_TYPE  :
      fprintf(stdout,"}");
      break; 
      case VOID_TYPE  :
      fprintf(stdout,"ERROR VOIDTYPE\n");
      break;
      case CLOSURE_TYPE  :
      fprintf(stdout,"CLOSURE_TYPE");
      break;
      case PRIMITIVE_TYPE :
      fprintf(stdout,"PRIMITIVE_TYPE");
      break;
    }
  }
  tree = cdr(tree);
}
  printf(")");//printing ( ) for each call at beginning/end respectively
}



void printTree(Value *tree){
  while (tree->type != NULL_TYPE){
    if(car(tree)->type == CONS_TYPE){
        printTreeH(car(tree));
    }else{
      printf(" ");
      switch(car(tree)->type){
      case STR_TYPE  :
      fprintf(stdout,"%s\"",car(tree)->s);
      break; 
      case INT_TYPE  :
      fprintf(stdout,"%i",car(tree)->i);
      break; 
      case DOUBLE_TYPE  :
      fprintf(stdout,"%f",car(tree)->d);
      break; 
      case SYMBOL_TYPE  :
      fprintf(stdout,"%s",car(tree)->s);
      break; 
      case OPEN_TYPE  :
      fprintf(stdout,"(:open\n");
      break; 
      case CLOSE_TYPE  :
      fprintf(stdout,"):close\n");
      break; 
      case PTR_TYPE  :
      fprintf(stdout,"POINTER\n");
      break; 
      case BOOL_TYPE  :
      if (car(tree)->i == 0){
        fprintf(stdout,"#f");
      } else {
        fprintf(stdout,"#t");
      }
      break; 
      case DOT_TYPE  :
      fprintf(stdout,".");
      break; 
      case NULL_TYPE  :
      fprintf(stdout,"()");
      break; 
      case SINGLEQUOTE_TYPE  :
      fprintf(stdout," \" ");
      break; 
      case OPENBRACKET_TYPE  :
      fprintf(stdout,"{");
      break; 
      case CONS_TYPE  :
      fprintf(stdout,"CONS\n");
      break; 
      case CLOSEBRACKET_TYPE  :
      fprintf(stdout,"}");
      break; 
      case VOID_TYPE :
      fprintf(stdout,"ERROR VOIDTYPE");
      break;
      case CLOSURE_TYPE  :
      fprintf(stdout,"CLOSURE_TYPE");
      break;
      case PRIMITIVE_TYPE :
      fprintf(stdout,"PRIMITIVE_TYPE");
      break;
    }
    }
    tree = cdr(tree);
  }

}




//(a b (c d)) (e f)

#endif


