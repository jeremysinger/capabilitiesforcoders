#include<cheri.h>
#include<stdio.h>
int main() {

//create a capability to a memory region 

int data[10] = {0};

cheri_object data_cap;

data_cap = cheri_build_data_cap(data, sizeof(data), CHERI_PERM_LOAD|CHERI_PERM_STORE);

//read and write access capability

int* cap_ptr = cheri_cast(int*, data_cap);

//read and write data 

cap_ptr[0] = 42; 

int value = cap_ptr[0];

printf("initial value: %d\n", value);

//let's simulate a security breach or a vulnerability detection 

//decide to revoke or write access to the memory region 

cheri_set_perms(&data_cap,CHERI_PERM_LOAD);//revoke write permissions

//attempting to write after revoking triggers an exception 

//uncommenting this line would trigger a capability violation exception 

//cap_ptr[1]=99;

//however,read access remains allowed 

int read_value = cap_ptr[1];

printf("read only value: %d\n", read_value);

return 0; 


}

