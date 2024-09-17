#include<cheri.h>
#include<stdio.h>
#include<string.h>

struct Student{

char name[100]; 

int age;

};

//component1

void component1(void* studentCapability){

struct Student* student = cheri_setbounds(studentCapability,sizeof(struct Student));

strcpy(student->name,"Alice");

student->age=20;

}

//component2

void component2(void* studentCapability){

struct Student* student = cheri_setbounds(studentCapability,sizeof(struct Student));

printf("componet2 reads: Name: %s, Age: %d\n", student->name, student->age);

}

int main (){

//memory allocation for the student data 

struct Student sharedStudent;

void* studentCapability = cheri_setbounds(&sharedStudent,sizeof(struct Student));

//components use capability to access the shared student data

component1(studentCapability);
component2(studentCapability);

return 0;

}
  

