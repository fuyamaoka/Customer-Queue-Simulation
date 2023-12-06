#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "Customer.h"
#include "QueueList.h"
#include "Register.h"
#include "RegisterList.h"

using namespace std;

// Function Declarations:

// Set mode of the simulation
string getMode();

// Register
void parseRegisterAction(stringstream &lineStream, string mode);
void openRegister(
    stringstream &lineStream,
    string mode);  // register opens (it is upto customers to join)
void closeRegister(stringstream &lineStream,
                   string mode);  // register closes 

// Customer
void addCustomer(stringstream &lineStream,
                 string mode);  // customer wants to join


// Helper functions
bool getInt(stringstream &lineStream, int &iValue);
bool getDouble(stringstream &lineStream, double &dValue);
bool foundMoreArgs(stringstream &lineStream);

// Extra functions
void calculate();

// Global variables
RegisterList *registerList; // holding the list of registers
QueueList *doneList; // holding the list of customers served
QueueList *singleQueue; // holding customers in a single virtual queue
double expTimeElapsed; // time elapsed since the beginning of the simulation

// List of commands:
// To open a register
// register open <ID> <secPerItem> <setupTime> <timeElapsed>
// To close register
// register close <ID> <timeElapsed>
// To add a customer
// customer <items> <timeElapsed>

int main() {
  registerList = new RegisterList();
  doneList = new QueueList();
  singleQueue = new QueueList();
  expTimeElapsed = 0;

  // Set mode by the user
  string mode = getMode();

  string line;
  string command;

  cout << "> ";  // Prompt for input
  getline(cin, line);

  while (!cin.eof()) {
    stringstream lineStream(line);
    lineStream >> command;
    if (command == "register") {
      parseRegisterAction(lineStream, mode);
    } else if (command == "customer") {
      addCustomer(lineStream, mode);
    } else {
      cout << "Invalid operation" << endl;
    }
    //cout << "TIME: " << expTimeElapsed << endl;
    cout << "> ";  // Prompt for input
    getline(cin, line);
  }

  cout << endl << "Finished at time " << expTimeElapsed << endl;
  cout << "Statistics:" << endl;

  calculate();

  // You have to make sure all dynamically allocated memory is freed 
  // before return 0

  delete registerList;
  registerList = NULL;
  delete doneList;
  doneList = NULL;
  delete singleQueue;
  singleQueue = NULL;

  return 0;
}

string getMode() {
  string mode;
  cout << "Welcome to ECE 244 Grocery Store Queue Simulation!" << endl;
  cout << "Enter \"single\" if you want to simulate a single queue or "
          "\"multiple\" to "
          "simulate multiple queues: \n> ";
  getline(cin, mode);

  if (mode == "single") {
    cout << "Simulating a single queue ..." << endl;
  } else if (mode == "multiple") {
    cout << "Simulating multiple queues ..." << endl;
  }

  return mode;
}

void addCustomer(stringstream &lineStream, string mode) {
  int items;
  double timeElapsed;
  if (!getInt(lineStream, items) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments." << endl;
    return;
  }
  // Depending on the mode of the simulation (single or multiple),
  // add the customer to the single queue or to the register with
  // fewest items

  expTimeElapsed += timeElapsed; 

  // Departure Code
  bool done = false;

  while ((registerList -> calculateMinDepartTimeRegister(expTimeElapsed)) != NULL && !done){

    Register *d = registerList -> calculateMinDepartTimeRegister(expTimeElapsed);
    //cout << "departtime: " << d -> calculateDepartTime() << endl << "TIMEELAPSED: " << expTimeElapsed << endl;
    //cout << "Before" << endl;
    if (((d -> calculateDepartTime()) - expTimeElapsed) <= 0){
      cout << "Departed a customer at register ID " << d -> get_ID() << " at " << d -> calculateDepartTime() << endl;     // Departed a customer at register ID 101 at 118
      d -> departCustomer(doneList);
      if (mode == "single"){
        if (singleQueue -> get_head()!= NULL){
          d -> get_queue_list() -> enqueue(singleQueue -> dequeue());
          cout << "Queued a customer with free register " << d ->get_ID() << endl;          //Queued a customer with free register 1010
        }
      }
    } else {
      done = true;
    }
  }

  Customer *c = new Customer((expTimeElapsed), items);

  cout << "A customer entered" << endl;    // A customer entered

  if (mode == "single"){

    Register *r = registerList -> get_free_register();

    if(r != NULL){
      r -> get_queue_list() -> enqueue(c);
      cout << "Queued a customer with free register " << r -> get_ID() << endl;      //Queued a customer with free register 101
    } else {
      singleQueue -> enqueue(c);
      cout << "No free registers" << endl;     // No free registers
    }

  } else if (mode == "multiple"){
    //cout << "here1" << endl;
    Register *r = registerList -> get_min_items_register();
    //cout << "here2" << endl;
    r -> get_queue_list() -> enqueue(c);
    //cout << "here3" << endl;
    cout << "Queued a customer with quickest register " << r -> get_ID() << endl;    // Queued a customer with quickest register 101
    //cout << "here4" << endl;

  }
}

void parseRegisterAction(stringstream &lineStream, string mode) {
  string operation;
  lineStream >> operation;
  if (operation == "open") {
    openRegister(lineStream, mode);
  } else if (operation == "close") {
    closeRegister(lineStream, mode);
  } else {
    cout << "Invalid operation" << endl;
  }
}

void openRegister(stringstream &lineStream, string mode) {
  int ID;
  double secPerItem, setupTime, timeElapsed;
  // convert strings to int and double
  if (!getInt(lineStream, ID) || !getDouble(lineStream, secPerItem) ||
      !getDouble(lineStream, setupTime) ||
      !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }

  // Check if the register is already open
  // If it's open, print an error message
  // Otherwise, open the register
  // If we were simulating a single queue, 
  // and there were customers in line, then 
  // assign a customer to the new register

  if (registerList -> foundRegister(ID)){
    cout << "Error: register " << ID << " is already open" << endl;               //Error: register 102 is already open.
    return;
  }

  expTimeElapsed += timeElapsed; 

  /// Departure Code
  bool done = false;

  while ((registerList -> calculateMinDepartTimeRegister(expTimeElapsed)) != NULL && !done){

    Register *d = registerList -> calculateMinDepartTimeRegister(expTimeElapsed);
    //cout << "departtime: " << d -> calculateDepartTime() << endl << "TIMEELAPSED: " << expTimeElapsed << endl;
    if (((d -> calculateDepartTime()) - expTimeElapsed) <= 0){
      cout << "Departed a customer at register ID " << d -> get_ID() << " at " << d -> calculateDepartTime() << endl;     // Departed a customer at register ID 101 at 118
      d -> departCustomer(doneList);
      if (mode == "single"){
        if (singleQueue -> get_head()!= NULL){
          d -> get_queue_list() -> enqueue(singleQueue -> dequeue());
          cout << "Queued a customer with free register " << d ->get_ID() << endl;          //Queued a customer with free register 1010
        }
      }
    } else {
      done = true;
    }
  }

  Register *r = new Register(ID,  secPerItem,  setupTime,  expTimeElapsed);
  registerList -> enqueue(r);
  
  cout << "Opened register " << r -> get_ID() << endl;      // Opened register 102

  if (mode == "single"){
    if (singleQueue -> get_head() != NULL){
      r -> get_queue_list() -> enqueue(singleQueue -> dequeue());
      cout << "Queued a customer with free register " << r -> get_ID() << endl;          // Queued a customer with free register 102
    }
  }
}

void closeRegister(stringstream &lineStream, string mode) {
  int ID;
  double timeElapsed;
  // convert string to int
  if (!getInt(lineStream, ID) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }
  // Check if the register is open
  // If it is open dequeue it and free it's memory
  // Otherwise, print an error message 

  expTimeElapsed += timeElapsed;

  // Departure Code
  bool done = false;

  while ((registerList -> calculateMinDepartTimeRegister(expTimeElapsed)) != NULL && !done){

    Register *d = registerList -> calculateMinDepartTimeRegister(expTimeElapsed);
    //cout << "departtime: " << d -> calculateDepartTime() << endl << "TIMEELAPSED: " << expTimeElapsed << endl;
    if (((d -> calculateDepartTime()) - expTimeElapsed) <= 0){
      cout << "Departed a customer at register ID " << d -> get_ID() << " at " << d -> calculateDepartTime() << endl;     // Departed a customer at register ID 101 at 118
      d -> departCustomer(doneList);
      if (mode == "single"){
        if (singleQueue -> get_head()!= NULL){
          d -> get_queue_list() -> enqueue(singleQueue -> dequeue());
          cout << "Queued a customer with free register " << d ->get_ID() << endl;          //Queued a customer with free register 1010
        }
      }
    } else {
      done = true;
    }
  }

  Register *r = registerList -> dequeue(ID);

  if (r == NULL){
    cout << "Error: register " << ID << " is not open" << endl;         // Error: register 103 is not open
    return;
  } else {
    delete r;
    cout << "Closed register " << ID << endl;                             //Closed register 101
  }
}

bool getInt(stringstream &lineStream, int &iValue) {
  // Reads an int from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  iValue = stoi(command);
  return true;
}

bool getDouble(stringstream &lineStream, double &dvalue) {
  // Reads a double from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  dvalue = stod(command);
  return true;
}

bool foundMoreArgs(stringstream &lineStream) {
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  } else {
    return true;
  }
}

void calculate(){

  double max = 0;
  double sum = 0;
  double num = 0;
  double avg;

  Customer *c = doneList -> get_head();

  while (c != NULL){
    
    if (((c -> get_departureTime()) - (c -> get_arrivalTime())) >= max){
      max = ((c -> get_departureTime()) - (c -> get_arrivalTime()));
    }
    //cout << "MAX: " << max << endl;
    sum += ((c -> get_departureTime()) - (c -> get_arrivalTime()));
    num++;
    c = c -> get_next();
    //cout << "SUM: " << sum << endl;
    //cout << "NUM: " << num << endl;
  }

  avg = (sum/num);

  cout << "  Maximum wait time: " << max << endl;
  cout << "  Average wait time: " << avg << endl;

  sum = 0;
  c = doneList -> get_head();

  while (c != NULL){

    sum += pow (((c -> get_departureTime()) - (c -> get_arrivalTime()) - avg), 2);

    c = c-> get_next();
  }

  cout << "  Standard Deviation of wait time: " << (sqrt ((sum/num))) << endl;
}