#include<stdio.h>

void  myFunction(int *ptr){
//trying to perform some  operations with ptr

}

int main() {

int array_data[5]={1,2,3,4,5};

int *ptr1 =array_data;

//pass ptr to myFunction

myFunction(ptr1);

//attempt to access 'array_data' through 'ptr1' again

int myValue =*ptr1; 

printf("myvalue : %d\n", myValue);

return 0;

} 
