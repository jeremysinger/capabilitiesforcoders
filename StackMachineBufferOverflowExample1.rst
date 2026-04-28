==========================
"Buffer Overflow" Example
==========================

Stack machine
-------------
For a typical C program, its memory is divided into different segments and stack is one of the segments. 
Stack: The stack is used for storing local variables defined inside functions, data related to function calls including:
for example, return address, etc... 

The use of stack memory example
-------------------------------

.. code-block:: C

    /**
    * Stack and function call 
    */
     int x = 50;
     int main()
    {
     int a = 5;   //stored on stack 
     int b = 10;  //stored on stack  
     static int y; 
     // allocation of memory on heap
     int * myPtr = (int *) malloc(2*sizeof(int));
     // values 20 and 25 are stored on heap
     myPtr[0]=20;    //stored on heap
     myPtr[1]=25;    //stored on heap

     free(myPtr);
     return(1);
    }
In the above program, the variables a and b inside the program are local variables and they are stored on the program stack.
Notice that the variable myPtr is also a local variable, it is also stored on the stack. However, myPtr as a pointer,
pointing to a block of memory and dynamically allocated using malloc();the values 20, and 25 assigned to myPtr[0],
and myPtr[1], are stored on the heap. 

The understanding of how data is arranged inside a program memory helps to understand how buffer overflow attack and
Vulnerability works.

Since buffer overflow can happen on both stack and heap, the ways to exploit them are somehow different and in the program
example below we focus on stack buffer overflow and it is important to understant what information that could be stored on 
the program memory stack, as shown in the above program example. 

Stack is used for storing data in function calls. Normally, when a program runs it executes a series of function calls.
Whenever a function is called, a space is created and allocated on the stack for the execution of that function. 


.. code-block:: function 
    /**
    * function code 
    */

      void myFunc(float x, float y)
    {
       float a, float b;
       a = x*y;
       b = x+y; 
    }

When myFunc is called, a block of memory space will be allocated on top of the program stack and the values of the arguments,
will also be pushed into the stack. 
When myFunc finishes, its return instructions, will be placed into the top of the program stack, and this is 
a return address. 

Stack Buffer Overflow Vulnerability and attack
-----------------------------------------------
When a program needs to copy a memory, data from one place need to be copied to another place. 
Before this happen, a program needs to allocate sufficient memory space to be able to copy for the destination buffer.
When more that are copied to the destination buffer than the amoout of allocated space, this creates a buffer overflow. 

.. code-block:: C 

   /**
   *  buffer overflow program 
   */
      
    #include <string.h>
    void foo(char *str)
    {

     char buffer[12];

     /* The following statement will result in a buffer overflow */

     strcpy(buffer, str);

    }
     int main()

    {

     char *str = "This is definitely longer than 12";
     foo(str);
     return 1;

    }
The program stack memory for the aboe code, the local array buffer [] in foo () function has 12 bytes of memory.
The function foo() uses strcpy() to copy the string from str to the target buffer. 
It is important to know that, normally this strcpy() function will not until it finds a number 0 in the above string. 
Given that the provided string is longer that 12 bytes, strcpy() will need to overwrite some portion of the program stack,
and this causes a buffer overflow. 
Some programming languages such as Java, can detect straightaway this simple mistake, when buffer is over-run. 
But pointer languages, such as C or C++, can find hard to detect it. 
As consequence, buffer overflow can cause a program to crash due to the corruption of the data beyond the buffer,
but could also enable adversaries or attackers to gain control of that program. 
