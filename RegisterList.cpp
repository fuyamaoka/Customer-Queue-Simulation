#include "RegisterList.h"

#include <iostream>

#include "Register.h"
using namespace std;

RegisterList::RegisterList() {
  head = nullptr;
  size = 0;
}

RegisterList::~RegisterList() {
  // Delete all registers in the list
  delete head;
}

Register* RegisterList::get_head() {return head;}

int RegisterList::get_size() { 
  // return number of registers 
  return size;
}


Register* RegisterList::get_min_items_register() {
  // loop all registers to find the register with least number of items
  
  int max = 0;

  Register* p = head;
  Register* YOU;

  for (int i = 0; i < size; i++){
    int num = 0;
    Customer *q =  p -> get_queue_list() -> get_head();

    while (q != NULL){
      num += q -> get_numOfItems();
      q = q -> get_next();
    }

    if (i == 0){
      max = num;
      YOU = p;
    } else if (num < max){
      max = num;
      YOU = p;
    }

    p = p -> get_next();
  }

  return YOU;
}

Register* RegisterList::get_free_register() {
  // return the register with no customers
  // if all registers are occupied, return nullptr

  Register *p = head;

  for (int i = 0; i < size; i++){
    
    Customer *q = p -> get_queue_list() -> get_head();
    
    if (q == NULL){
      return p;
    }

    p = p -> get_next();
  }

  return NULL;
}

void RegisterList::enqueue(Register* newRegister) {
  // a register is placed at the end of the queue
  // if the register's list is empty, the register becomes the head
  // Assume the next of the newRegister is set to null
  // You will have to increment size 
  
  if (head == NULL){
    head = newRegister;
    size++;
    return;
  }

  Register *p = head;

  while (p -> get_next() != NULL){
    p = p -> get_next();
  }

  p -> set_next(newRegister);
  size++;
}

bool RegisterList::foundRegister(int ID) {
  // look for a register with the given ID
  // return true if found, false otherwise

  if (head == NULL){
    return false;
  }

  Register *p = head;

  while (p != NULL && p -> get_ID() != ID){
    p = p -> get_next();
  }

  if (p == NULL){
    return false;
  } else {
    return true;
  }
}

Register* RegisterList::dequeue(int ID) {
  // dequeue the register with given ID

  Register *p = head;
  Register *q = NULL;

  while ( p != NULL && p -> get_ID() != ID){
    q = p;
    p = p -> get_next();
  }

  if (p == NULL){
    return NULL;
  } else if (q == NULL){
    head = p -> get_next();
    p -> set_next(NULL);
    size--;
    return p;
  } else {
    q -> set_next(p -> get_next());
    p -> set_next(NULL);
    size--;
    return p;
  }
  // return the dequeued register
  // return nullptr if register was not found
}

Register* RegisterList::calculateMinDepartTimeRegister(double expTimeElapsed) {
  // return the register with minimum time of departure of its customer
  // if all registers are free, return nullptr
  
  Register *r = head;
  double low = 0;
  bool first = true;
  Register *YOU = NULL;

  while (r != NULL){

    if (r -> calculateDepartTime() == -1){

    } else if (first){
      low = ((r -> calculateDepartTime()) - expTimeElapsed);
      YOU = r;
      first = false;
      //cout << "1st: ";
    } else if (((r -> calculateDepartTime()) - expTimeElapsed) <= low){
      low = ((r -> calculateDepartTime()) - expTimeElapsed);
      YOU = r;
      //cout << "2nd ";
    }
    //cout << "LOW: " << low << endl;
    r = r -> get_next();
  }
  //cout << "------------" << endl;
  return YOU;
}

void RegisterList::print() {
  Register* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
