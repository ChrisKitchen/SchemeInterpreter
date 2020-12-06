#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "linkedlist.h"
#include "value.h"
#include "talloc.h"

Value *makeNull(){
  Value *node;
  node = talloc(sizeof(Value));
  node->type = NULL_TYPE;
  return node;
}

Value *cons(Value *newCar, Value *newCdr){
  Value *node;
  node = talloc(sizeof(Value));
  node->type = CONS_TYPE;
  node->c.car = newCar;
  node->c.cdr = newCdr;
  
  return node;
}

Value *copynode(Value *node){
  Value *new_node;
  new_node = talloc(sizeof(Value));
  *new_node = *node;
  return new_node;
}

Value *car(Value *list){
  return list->c.car;
}

Value *cdr(Value *list){
  return list->c.cdr;
}

Value *reverse(Value *list){
  Value *linkedList = makeNull();
  //int i = 1;

  while(list->type != NULL_TYPE){
    Value *new_node;
    // new_node = cons(copynode(car(list)), linkedList);
    //fprintf(stdout,"%i\n",i);
    new_node = cons(car(list), linkedList);
    new_node->type = list->type; 
    
    if(car(new_node)->type == STR_TYPE){
      // car(new_node)->s = talloc(strlen(car(list)->s) + 1);
      car(new_node)->s = car(list)->s;
      // strcpy(car(new_node)->s, car(list)->s);
    }
    
    list = cdr(list);
    linkedList = new_node;
    //i++;
  }

  return linkedList;
}

void display(Value *list){
  while(list->type != NULL_TYPE){
    printf("the contents of cur_node are: ");
    switch(car(list)->type){
      case STR_TYPE  :
      printf("%s\n",car(list)->s);
      break; 
      case INT_TYPE  :
      printf("%i\n",car(list)->i);
      break; 
      case DOUBLE_TYPE  :
      printf("%f\n",car(list)->d);
      break; 
      case NULL_TYPE  :
      printf("NULL\n");
      break; 
      case CONS_TYPE  :
      printf("CONS\n");
      break; 
      case PTR_TYPE   :
      printf("%p\n",car(list)->p);
      break;
      case DOT_TYPE  :
      printf("CONS\n");
      break; 
      case BOOL_TYPE  :
      printf("CONS\n");
      break; 
      case OPEN_TYPE  :
      printf("CONS\n");
      break; 
      case CLOSE_TYPE  :
      printf("CONS\n");
      break; 
      case SYMBOL_TYPE  :
      printf("CONS\n");
      break; 
      case OPENBRACKET_TYPE  :
      printf("CONS\n");
      break; 
      case SINGLEQUOTE_TYPE  :
      printf("CONS\n");
      break; 
      case CLOSEBRACKET_TYPE  :
      printf("CONS\n");
      break; 
      case VOID_TYPE :
      printf("ERROR: VOIDTYPE");
      texit(0);
      break;
      case CLOSURE_TYPE:
      break;
      case PRIMITIVE_TYPE:
      break;
    }
    list = cdr(list);
  }
}

bool isNull(Value *value){
  if (value->type == NULL_TYPE){
    return true;
  }
  return false;
}

int length(Value *value){
  int counter = 0;
  while(value->type != NULL_TYPE){
    value = cdr(value);
    counter++;
  }
  return counter;
}
