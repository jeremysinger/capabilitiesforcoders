/*

 How do I printf a capability value 
 we use calloc() and still producing same results
 However, the memory allocation with calloc() will be initialized to zero

 by Joseph Kaberuka
*/

#include<stdio.h>
#include<stdlib.h>

int main() {

void *x =(void *)calloc(1, 1);
printf("The address in this capability is %p\n", x);
printf("The full capability (incl metadata) is %#p\n", x);
// Freeing the allocated memory
free(x);

return 0;

}
