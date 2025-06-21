#include<cheri.h>

struct capData {

char data[100];

};

void component1(){

struct capData* sharedData =(struct capData*)cheri_malloc(sizeof(struct capData));

strcpy(sharedData->data, "Data from component1");

//.. some code to pass sharedData to component2

}

//component2

void component2(struct capData* sharedData) {

printf("component2 read: %s\n",sharedData ->data);

}

int main() {


struct capData* sharedData = NULL;

component1(&sharedData);
component2(sharedData);//ensures that capability restricts access

cheri_free(sharedData);

return 0;

}


