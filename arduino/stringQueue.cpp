#include <Arduino.h>

#include "stringQueue.h"


class stringQueue stringQueue;

//------------------------------------------------
//------------------------------------------------
//------------------------------------------------
stringQueue::stringQueue()
{
  reset();
}
//------------------------------------------------
int stringQueue::count(void)
{
 if (_tail==-1) return 0;
 if (_tail >= _head)
   return _tail - _head+1;
 else 
   return MAX_STRINGS_IN_QUEUE - _head + _tail+1;
}
//------------------------------------------------
int stringQueue::push(char* in)
{
 if (in==0)       return -1;
 if (count()>=MAX_STRINGS_IN_QUEUE) return -1;

 if (_tail == -1) {_tail=0; _head=0;}
 else _tail=(_tail+1 % MAX_STRINGS_IN_QUEUE);

 strncpy(_q[_tail],in,MAX_CHARS_IN_STRING);
 return _tail;
}

//------------------------------------------------
char *stringQueue::pop(char* out)
{
 if (0==peek(out)) return 0;

 _q[_head][0]=0;
 if (count()==1) {_head=-1; _tail=-1;}
 _head=(_head+1 % MAX_STRINGS_IN_QUEUE);
 return out;
}
//------------------------------------------------
char *stringQueue::peek(char* out)
{
 if (out==0)       return 0;
 if (count()==0)   {out[0]=0; return 0;}

 strncpy(out,_q[_head],MAX_CHARS_IN_STRING); 
 return out;
}
//------------------------------------------------
char *stringQueue::peek(int pos, char* out)
{
 if (out==0)       return 0;
 if ((count()==0) || (pos<0) || (pos>= MAX_CHARS_IN_STRING) ){
   out[0]=0; return 0;
 }
 if (_q[pos] ==0) {
   out[0]=0; return 0;
 }

 strncpy(out,_q[pos],MAX_CHARS_IN_STRING); 
 return out;
}
//------------------------------------------------
//------------------------------------------------
void stringQueue::reset(void)
{
  for (int i=0;i<MAX_STRINGS_IN_QUEUE;i++)  _q[i][0]=0;
  _head=-1;
  _tail=-1;
}


