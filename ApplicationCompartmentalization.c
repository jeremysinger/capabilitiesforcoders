#include<cheri.h>
#include<stdio.h>

void componentA(){

char buffer[128];

//Assume cap_A is a capability  for component A's memory

_capability char* buffer_cap = cheri_setbounds(buffer, sizeof(buffer));

gets(buffer_cap); //protected by componentA's capability

printf("componentA : You entered : %s\n", buffer);

}

void componentB (){

int secret_data =42;

//Assume cap_B is a capability for component B's memory

// componentB has no access to componentA's memory 



_capability int* secret_data_cap = cheri_setbounds(&secret_data, sizeof(secret_data));

printf("componentB secret data is %d\n", *secret_data_cap);

}

int main() {

// component A and component B are isolated with separate capabilities 

componentA();
componentB();

return 0;

} 


