#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/mman.h>

#define USE_LB_SEALED_CAP 1

/**
* Wrappable function type.
*/
typedef void *(cmpt_fun_t)(void* arg);

#if (USE_LB_SEALED_CAP == 1)
// ...
#else
// ...
#endif

static const cmpt_t *create_cmpt(cmpt_fun_t *target, unsigned stack_pages);

//create a new function that prints a name and pass it as a target function to the create_cmpt f(x)
// The function prints for example my name is Joseph Kaberuka, this takes just a single argument for example: unused

static void *printMyName(void *unused)
{
    // Your name
    printf("My name is Joseph Kaberuka\n");
    return NULL;
}

//create_cmpt function, create a compartment for printMyName and allocate a small amount of memory for the compartment's stack


int main(int argc, char *argv[])
{
    const cmpt_t *cmpt = create_cmpt(printMyName, 1 /* 1 page should be enough */);

    printf("before...\n");
    printf("csp: %s\n", cap_to_str(NULL, cheri_csp_get()));

//cmpt_call executes printMyName function within the compartment  

    cmpt_call(cmpt, NULL);

    printf("after...\n");
    printf("csp: %s\n", cap_to_str(NULL, cheri_csp_get()));

    return 0;
}
