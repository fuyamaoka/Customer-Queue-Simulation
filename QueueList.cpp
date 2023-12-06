#include "QueueList.h"

#include "Customer.h"

QueueList::QueueList() { head = nullptr; }

QueueList::QueueList(Customer* customer) { head = customer; }

QueueList::~QueueList() {
  delete head;
}

Customer* QueueList::get_head() {return head;}

void QueueList::enqueue(Customer* customer) {
  // a customer is placed at the end of the queue
  // if the queue is empty, the customer becomes the head

  if (head == NULL){
    head = customer;
    return;
  }

  Customer *p = head;

  while ((p->get_next()) != NULL){
    p = p -> get_next();
  }

  p -> set_next(customer);
  
}

Customer* QueueList::dequeue() {
  // remove a customer from the head of the queue 
  // and return a pointer to it
  
  if (head == NULL){}                               // DO I NEED THIS?

  Customer *p = head;

  head = head -> get_next();
  p -> set_next(NULL);

  return p;
}

int QueueList::get_items() {
  // count total number of items each customer in the queue has

  if (head == NULL){
    return 0;
  }

  int n;
  Customer *p = head;

  while ((p -> get_next()) != NULL) {
    n = n + p -> get_numOfItems();
    p = p -> get_next();
  }

  return (n + p -> get_numOfItems());
}

bool QueueList::empty_queue() {
  // if the queue is empty, return false
  // if the queue is not empty, delete all customers
  // and return true
  
  if (head == NULL){
    return false;
  }

  delete head;
  head = NULL;

  return true;
}

void QueueList::print() {
  // print customers in a queue
  Customer* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
