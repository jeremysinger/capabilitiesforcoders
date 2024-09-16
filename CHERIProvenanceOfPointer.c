#include<cheri.h>
#include<stdio.h>

int main(){

int myArray[10];

int *ptr = &myArray[0]; // storing the pointer as a capability

//perform a capability safe operation  - accessing a valid element 

int myValue = *ptr; 

// access within bounds

int *new_ptr = ptr + 5; 

printf("valid value : %d\n", myValue); 

return 0;

}
