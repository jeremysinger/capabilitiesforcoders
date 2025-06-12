===============================================
"StackMachineBufferOverflowDetection" Example
===============================================

Buffer Overflow Protection 
--------------------------
For stack-based buffer overflow attackers or adversaries, need to modify the return address. 
This means, if we can detect this modification before returning from a f(x) we can prevent it. There are different ways to achieve this
1. We can store a copy of the return address not on the stack, so that it cannot be overwitten via a buffer overflow
2. We can put a protection or a countermeasure strategy between a return address and the buffer 

N.B Though we are not implementing any of those measures, Morello detects this vulnerability, terminates and prevents it 


.. code-block:: C
    /*
     Buffer overflow detection 
   */
    #include <stdio.h>
    #include <stdlib.h>

   void foo(char *str)
   {
   char buffer[12];
   /* Buffer Overflow Vulnerability */
   strcpy(buffer, str);
   }
   int main(int argc, char *argv[])
   {
   foo(argv[1]);
   printf("returned \n\n");
   return 0;
   }
Morello detects buffer overflow again in this program and terminates it