#include "Register.h"

#include <iostream>

//
using namespace std;
//
Register::Register(int id, double timePerItem, double overhead,
                   double entryTime) {
  ID = id;
  secPerItem = timePerItem;
  overheadPerCustomer = overhead;
  availableTime = entryTime;
  next = nullptr;           // no other registers yet
  queue = new QueueList();  // no customers in line yet, but has to initialize a
                            // queue
}

Register::~Register() {delete queue; delete next;}

QueueList* Register::get_queue_list() {return queue;}

Register* Register::get_next() {return next;}

int Register::get_ID() {return ID;}

double Register::get_secPerItem() {return secPerItem;}

double Register::get_overheadPerCustomer() {return overheadPerCustomer;}

double Register::get_availableTime() {return availableTime;}

void Register::set_next(Register* nextRegister) {next = nextRegister;}


void Register::set_availableTime(double availableSince) {
  availableTime = availableSince;
}

double Register::calculateDepartTime() {
  // Get the departure time of the first customer in the queue
  // returns -1 if no customer is in the queue
  
  if ((queue -> get_head()) == NULL){
    return -1;
  }

  double c;
  Customer *p = queue -> get_head();

  if ((p -> get_arrivalTime()) > availableTime){
    c = p -> get_arrivalTime() + overheadPerCustomer + (secPerItem * p -> get_numOfItems());
    //cout << "DEPART: " << c << endl;
    return c;
  } else {
    c = availableTime + overheadPerCustomer + (secPerItem * p -> get_numOfItems());
    //cout << "DEPART: " << c << endl;
    return c;
  }
}

void Register::departCustomer(QueueList* doneList) {
  // dequeue the head, set last dequeue time, add to doneList,
  // update availableTime of the register

  availableTime = calculateDepartTime();

  Customer *p = queue -> dequeue();

  p -> set_departureTime(availableTime);

  doneList -> enqueue(p);

  //cout << "Available: " << availableTime <<endl;
}

void Register::print() {
  std::cout << "Register ID: " << ID << std::endl;
  std::cout << "Time per item: " << secPerItem << std::endl;
  std::cout << "Overhead per customer: " << overheadPerCustomer << std::endl;
  if (queue->get_head() != nullptr) {
    std::cout << "Queue has customers: \n";
    queue->print();
  }
}
