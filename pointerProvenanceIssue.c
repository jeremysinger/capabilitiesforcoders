
#include<stdint.h>
#include<stdio.h>

int main () {

int myArray[10];

uintptr_t ptr =  (uintptr_t)&myArray[0]; //storing the address as uintptr_t

//performing an invalid operation - accessing a non existent element 

uintptr_r myPtr = ptr + sizeof(int)* 20; 

//trying to accessing the memory using an invalid pointer 

int myValue = *(int*)myPtr   // no memory safety checks 

printf("invalid value : %d\n ",myValue);


return 0;

}  
 


