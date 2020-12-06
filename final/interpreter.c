#include <stdlib.h>
#include <stdio.h>
#include "tokenizer.h"
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include <assert.h>
#include <string.h>
// #include <math.h>
#include "parser.h"

int isTrue(Value *val){

  if (val->type == BOOL_TYPE){
    if(val->i == 1){
      return 1;
    }else if(val->i == 0){
      return 0;
    }
  } else{
    printf("Evaluation Error");
    texit(0);
  }

  return 0;
}


Value *lookupSymbol(Value *symbol, Value *bindings, Frame *frame){
  while(frame != NULL){
    //printf("hello");
    while(bindings->type != NULL_TYPE){
     // printf("nice");
      Value *bargs = car(bindings);
      if((!strcmp(car(bargs)->s,symbol->s))){
       // printf("gggg");
        return cdr(bargs);
      }else{
        //printf("looking for: %s ",symbol->s);
       // printf("we found: %s ",car(bargs)->s);
        bindings = cdr(bindings);
      }
    }
    if(frame->parent != NULL){
      frame = frame->parent;
      bindings = frame->bindings;
    }
    else{
      printf("Evaluation Error: symbol undefined");
      texit(0);
    }
  }
  
  printf("Evaluation Error: symbol undefined");
  texit(0);
  return NULL;
}
int lookupSymbolTwo(Value *symbol, Value *bindings, Frame *frame){
  while(frame != NULL){
    while(bindings->type != NULL_TYPE){
      if(!strcmp(car(car(bindings))->s,symbol->s)){

        return 1;
      }
      
      bindings = cdr(bindings);
    }
      return 0;
  }
  return 0;
}

Value *eval(Value *tree, Frame *frame);
Frame *makeNullFrame();

Value *ifif(Value *tree, Frame *frame){
  if(length(tree) != 3){
    printf("Evaluation Error: not 3 stuffs");
    texit(0);
  }
  
  Value *evalbool = eval(car(tree),frame);
  if (evalbool->type != BOOL_TYPE){
    printf("Evaluation Error: not bool type : %i", evalbool->type);
    texit(0);
  }
  if (evalbool->i == 1){
    return eval(car(cdr(tree)),frame);
  } else {
    return eval(car(cdr(cdr(tree))),frame);
  }
}

  
int cellForm(Value *value){
  if(value == NULL){
    return 1;
  }
  if(value->type == NULL_TYPE){
    printf("helloooo");
    return 1;
  }
  return 0;
}

// Frame *globalFrame;

Value *ifDefine(Value *tree, Frame *frame){
  //similar to let
  //different in two main ways:
    //1.does not create a new environment frame
    //adds to global frame
    //2.does not print anything


  //return a Value with type VOID_TYPE
  Value *noVal = makeNull();
  noVal->type = VOID_TYPE;

  Value *newBinding;
  newBinding = makeNull();
  
  if(length(tree)!= 2){
    printf("Evaluation Error: wrong tree length: %i",length(tree));
    texit(0);
  }

  if(car(cdr(tree)) == NULL){
    printf("Evaluation Error: empty define");
    texit(0);
  }

  if(car(tree)->type != SYMBOL_TYPE){
    printf("Evaluation Error: first define value not symbol type");
    texit(0);
  }
  Frame *globalFrame = frame;
  while(globalFrame->parent != NULL){
    globalFrame = globalFrame->parent;
  }  

  // Value *theX = makeNull();
  // Value *theSeven = makeNull();
  // theX -> s = "s";
  // theSeven -> i = 7;
  // newBinding = cons(theX, the7);

  newBinding = cons(car(tree), eval(car(cdr(tree)), frame));
   
  //lookupSymbol(tree,frame->bindings, frame);
  globalFrame->bindings = cons(newBinding,globalFrame->bindings);
  
  
  
  //(let ((x 7)) x 3 5) 
  
  // globalFrame->parent = frame;
  return noVal;

  // return noVal;

  //(define x 7)

  //(define x (if #t 2 3)) -> (returns void)
  //(display x) -> 7
  
}

// Value *newClosure;
//((lambda (f g 6) 23))
//((lambda () (if #t 5 7)))
Value *ifLambda(Value *tree, Frame *frame){

  //go through all parameters and store them in a closure w/ type CLOSURE_TYPE
  Value *newClosure;
  newClosure = makeNull();


  if (length(tree) != 2){
    printf("Evaluation error: length");
    texit(0);
  }

  Value *tempCons = car(tree);
  while (tempCons->type != NULL_TYPE){
    if (car(tempCons)->type != SYMBOL_TYPE){
      printf("Evaluation error: first param not symbol type");
      texit(0);
    }

    tempCons = cdr(tempCons);
  }
  Value *i = car(tree);


  newClosure->type = CLOSURE_TYPE;
  newClosure->cl.paramNames = car(tree);
  newClosure->cl.functionCode = car(cdr(tree));
  newClosure->cl.frame = frame;
  return newClosure;
}



Value *apply(Value *function, Value *args){

  //call after function has been evaluated
  
  
  //Construct a new frame whose parent frame is the environment stored in the closure.
  Frame *newFrame = makeNullFrame();
  newFrame->parent = function->cl.frame;

  //Add bindings to the new frame mapping each formal parameter (found in the closure) to the corresponding actual parameter (found in args).
  Value *params = function->cl.paramNames;
  
  

  while(params->type != NULL_TYPE && strcmp(params->s, "lambda")){
    //printf("Param Pointer : %s", params->s);

    Value *newBinding = makeNull();
    newBinding = cons(car(params), car(args));

    newFrame->bindings = cons(newBinding, newFrame->bindings);

    //update paramNames and args
    params = cdr(params);
    args = cdr(args);
  }
  //Evaluate the function body (found in the closure) with the new frame as its environment, and return the result of the call to eval.
  return eval(function->cl.functionCode,newFrame);
}

Value *iflet(Value *tree, Frame *frame){
  Value *newBinding;
  newBinding = makeNull();
  Frame *newFrame = makeNullFrame();

  Value *cell;
  cell = makeNull();
  cell = car(tree);
  if(car(cdr(tree)) == NULL){
       printf("Evaluation Error: empty let");
      texit(0);
    }
  while (cell->type != NULL_TYPE){
    if(cell->type != CONS_TYPE){
      printf("Evaluation Error: wrong cell type");
      texit(0);
    }
    if(car(cell)->type == SYMBOL_TYPE){
      if(cellForm(cdr(cell))){
        printf("Evaluation Error: wrong cell form");
        texit(0);
      } 
    }
    if(car(cell)->type != CONS_TYPE){
      printf("Evaluation Error: wrong cell form");
      texit(0);
    }
    if(length(car(cell))!= 2){
      printf("Evaluation Error: wrong cell length");
      texit(0);
    }
    if(newFrame->bindings->type != NULL_TYPE && lookupSymbolTwo(car(car(cell)), newFrame->bindings, frame)){
      printf("Evaluation Error: duplicate variable in let");
      texit(0);
    }
    if(car(car(cell))->type != SYMBOL_TYPE){
      printf("Evaluation Error: first cell value not symbol type");
      texit(0);
    }
    if(car(cdr(car(cell)))->type == SYMBOL_TYPE){
      newBinding = cons(car(car(cell)),lookupSymbol(car(cdr(car(cell))),frame->bindings,frame));
    }else{
      newBinding = cons(car(car(cell)), eval(car(cdr(car(cell))), frame));
    }
    newFrame->bindings = cons(newBinding,newFrame->bindings);
    
    
    cell = cdr(cell);
  }
  Value *Ret = tree;
  while (cdr(Ret)->type != NULL_TYPE){
    Ret = cdr(Ret);
  }
  
  newFrame->parent = frame;
  return eval(car(Ret),newFrame);
}


Value *ifQuote(Value *tree, Frame *frame){
  if (car(tree)->type == INT_TYPE && length(tree) != 1){
    printf("Evaluation error");
    texit(0);
  } 
  return car(tree);
}
Value *evalEach(Value *tree, Frame *frame){


  Value *ret;
  ret = makeNull();
  while(tree->type != NULL_TYPE){
    ret = cons(eval(car(tree),frame),ret);
    tree = cdr(tree);
  }
ret = reverse(ret);
return ret;
}

Value *ifNull(Value *tree, Frame *frame){
  Value *ret = makeNull();
  ret->type = BOOL_TYPE;
  // printf("TREE TYPE!! %i !!\n", car(tree)->type);
  if(length(tree) != 1){
    printf("Evaluation Error: ifNull wrong number of params");
    texit(0);
  }
  if(eval(car(tree), frame)->type == NULL_TYPE){
    ret->i = 1;
  }else{ret->i = 0;}  
  return ret;
}

Value *ifCar(Value *tree, Frame *frame){
  if(tree->type != CONS_TYPE){
    printf("Evaluation Error: CAR ERROR");
    texit(0);
  }
  if(length(tree) != 1){
    printf("Evaluation Error: Car too many args");
    texit(0);
  }
  
  if(eval(car(tree),frame)->type == CONS_TYPE){
    return car(eval(car(tree),frame));
  }else{
    printf("Evaluation Error:Car, Not a Cons Type");
    texit(0);
    return makeNull();
  }
}
Value *ifCdr(Value *tree, Frame *frame){
  if(tree->type != CONS_TYPE){
    printf("Evaluation Error: CDR ERROR");
    texit(0);
  }
  if(length(tree) != 1){
    printf("Evaluation Error: CDR ERROR: too many args");
    texit(0);
  }
  if(eval(car(tree),frame)->type == CONS_TYPE){
    return cdr(eval(car(tree),frame));
  }else{
    printf("Evaluation Error: CDR ERROR: Not a Cons Type");
    texit(0);
    return makeNull();
  }
}

Value *ifCons(Value *tree, Frame *frame){
  if(length(tree) != 2){
    printf("Evaluation Error: CONS ERROR: wrong number of params");
    texit(0);
  }
  Value *newtree = cons(eval(car(tree),frame),eval(car(cdr(tree)),frame));
  return newtree;
} 
//(7 6 5)
//("hello" . "jjjj")

Value *ifAdd(Value *tree, Frame *frame){
  
  Value *keep_track = tree;
  Value *newTree = makeNull();
  double additive; 
  double solution = 0;
 
  while(keep_track->type != NULL_TYPE){
    if(eval(car(keep_track),frame)->type == INT_TYPE){
      additive = eval(car(keep_track),frame)->i;
      //newTree->type = INT_TYPE;
    } else if(eval(car(keep_track),frame)->type == DOUBLE_TYPE){
      additive = eval(car(keep_track),frame)->d;
      //newTree->type = DOUBLE_TYPE;
    } else{
      printf("Evaluation Error: ADD ERROR: Wrong type");
      texit(0);
    }
    
    solution = solution + additive;
    keep_track = cdr(keep_track);
  }
  newTree->d = solution;
  newTree->type = DOUBLE_TYPE;
  return newTree;
}
Value *eval(Value *tree, Frame *frame) {

  if (tree->type == NULL_TYPE){
    texit(0);
    return NULL;
  }
  switch(tree->type){
    case INT_TYPE:
      return tree;

    case VOID_TYPE:
      return NULL;
      
    case DOUBLE_TYPE:
      return tree;

    case STR_TYPE:
      return tree;

    case SYMBOL_TYPE:

      return lookupSymbol(tree,frame->bindings, frame);
    
    case BOOL_TYPE:
      return tree;

    case CONS_TYPE:
      if(car(tree)->type == SYMBOL_TYPE) {
        if (!strcmp(car(tree)->s,"if")){
          return ifif(cdr(tree), frame);
        } else if (!strcmp(car(tree)->s,"let")){
          return iflet(cdr(tree), frame);
        } else if (!strcmp(car(tree)->s, "quote")){
          return ifQuote(cdr(tree), frame);
        } else if (!strcmp(car(tree)->s, "define")){
          return ifDefine(cdr(tree), frame);
        } else if(!strcmp(car(tree)->s, "lambda")){
          return ifLambda(cdr(tree), frame);
        } else if(!strcmp(car(tree)->s, "null?")){
          return ifNull(cdr(tree), frame);
        } else if(!strcmp(car(tree)->s, "car")){
          return ifCar(cdr(tree), frame);
        } else if(!strcmp(car(tree)->s, "cdr")){
          return ifCdr(cdr(tree), frame);
        } else if(!strcmp(car(tree)->s, "cons")){
          return ifCons(cdr(tree), frame);
        } else if(!strcmp(car(tree)->s, "+")){
          return ifAdd(cdr(tree), frame);
        } 
        else{
          Value *evaledOperator = eval(car(tree), frame);
          Value *evaledArgs = evalEach(cdr(tree), frame);
          return apply(evaledOperator,evaledArgs);
        }
      } else if(car(tree)->type == CONS_TYPE){
        Value *evaledOperator = eval(car(tree),frame);
        Value *evaledArgs = evalEach(cdr(tree), frame);
        return apply(evaledOperator,evaledArgs);
      } else {
        printf("%i",car(tree)->i);
        return eval(car(tree),frame);
      }

    default: 
      return NULL;
  } 
}

  

Frame *makeNullFrame(){
  Frame *frame;
  Value *newBinding = makeNull();
  frame = talloc(sizeof(Frame));
  frame->bindings = newBinding;
  frame->parent = NULL;
  return frame;
}

void displayEval(Value *result){
  switch(result->type){
      case STR_TYPE  :
      fprintf(stdout,"%s\" ",result->s);
      break; 
      case INT_TYPE  :
      fprintf(stdout,"%i ",result->i);
      break; 
      case DOUBLE_TYPE  :
      // int tempNumber = (int) result->d;
      if (result->d == (int) result->d){
        fprintf(stdout,"%i ", (int) result->d);
      } else {
        fprintf(stdout,"%1.4f ",result->d);
      }
      break; 
      case SYMBOL_TYPE  :
      fprintf(stdout,"%s ",result->s);
      break; 
      case OPEN_TYPE  :
      fprintf(stdout,"(:open\n");
      break; 
      case CLOSE_TYPE  :
      fprintf(stdout,"):close\n");
      break; 
      case PTR_TYPE  :
      fprintf(stdout,"POINTER");
      break; 
      case BOOL_TYPE  :
      if (result->i == 0){
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
      fprintf(stdout,"{\n");
      break; 
      case CONS_TYPE  :
      
      printf("("); 

      while(result->type != NULL_TYPE){  
        if (result->type == CONS_TYPE){     
          displayEval(car(result));
          result = cdr(result);
        } else {
          printf(" . ");
          displayEval(result);
          break;
        }
      }
      printf(") ");
      break; 
      case CLOSEBRACKET_TYPE  :
      fprintf(stdout,"}\n");
      break; 
      case VOID_TYPE  :
      fprintf(stdout," ");
      break;
      case CLOSURE_TYPE  :
      fprintf(stdout,"#<procedure>");
      break;
      default: 
      printf("ERROR\n");
      break;
      case PRIMITIVE_TYPE:
      fprintf(stdout,"PRIMITIVE_TYPE");
      break;
    }
}


void interpret(Value *tree){

  Frame *frame;
  frame = makeNullFrame();
  while(tree->type != NULL_TYPE){
    Value *result = eval(car(tree), frame);
    displayEval(result);
    printf("\n");
    tree = cdr(tree);
  }

  texit(0);
}
