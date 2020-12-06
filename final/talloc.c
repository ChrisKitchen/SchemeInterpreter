#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "value.h"
#include "talloc.h"


Value *tcar(Value *list){
  return list->c.car;
}

Value *tcdr(Value *list){
  return list->c.cdr;
}

Value *tmakeNull(){
  Value *node;
  node = malloc(sizeof(Value));
  node->type = NULL_TYPE;
  return node;
}


Value *tcons(Value *newCar, Value *newCdr){
  Value *node;
  node = malloc(sizeof(Value));
  node->type = CONS_TYPE;
  node->c.car = newCar;
  node->c.cdr = newCdr;
  
  return node;
}

Value *tallocList;

void *talloc(size_t size){
  Value *pointer = malloc(size);
  
  if(tallocList == NULL){
    tallocList = tmakeNull();
  }
  
  tallocList = tcons(pointer, tallocList);
  return pointer;
}

void tfree(){
  Value *temp_node;
  while(tallocList->type != NULL_TYPE){
    temp_node = tcdr(tallocList);
    free(tcar(tallocList));
    free(tallocList);
    tallocList = temp_node;
  }

  assert(tallocList != NULL);


  free(tallocList);
  tallocList = NULL;
  // printf("%i\n",tcar(tallocList));
}

void texit(int status){
  tfree();
  exit(status);
}