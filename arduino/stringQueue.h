#ifndef STRINGQUEUE_h
#define STRINGQUEUE_h

#include "Arduino.h"


#define MAX_STRINGS_IN_QUEUE   7
#define MAX_CHARS_IN_STRING    50

class stringQueue
{
 public:
  stringQueue();
  int push(char* in);
  char *pop(char* out);
  char *peek(int pos,char* out);
  char *peek(char* out);
  int getHead(){return _head;}
  int getTail(){return _tail;}
  int count();
  void reset();

 private:
  char _q[MAX_STRINGS_IN_QUEUE][MAX_CHARS_IN_STRING];
  int _head;
  int _tail;
};

//extern class stringQueue stringQueue;
#endif


