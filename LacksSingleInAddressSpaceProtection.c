#include<stdio.h>
#include<string.h>

char sharedData[100];

//component1
void component1(){

strcpy(sharedData, "Data from component1");

}

//component2
void component2(){

printf("component2 reads: %s\n", sharedData);

}

int main(){

component1();

component2();

return 0;

}


