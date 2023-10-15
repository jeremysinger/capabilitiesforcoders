======================
"Hello World" Example
======================

Create Compartment
------------------
First we define a struct to describe the compartment.

.. code-block:: C

    /**
    * Compartment descriptor
    */
    typedef struct {
        void *entry;    // sentry for entry into compartment
        void *exit;     // sentry for return from compartment
        void *stack;    // callee's stack
        void *target;   // target function (sentry)
    } cmpt_desc_t;


Then we can define a helper function to create a compartment.

.. code-block:: C

    static const cmpt_t *create_cmpt(cmpt_fun_t *target, unsigned stack_pages)


Inside this helper function, we first allocate some memory as the callee function's stack.

.. code-block:: C

    #define STACK_PERMS (PERM_GLOBAL | READ_CAP_PERMS | WRITE_CAP_PERMS)

    size_t pgsz = getpagesize();
    size_t sz = pgsz * stack_pages;

    void *stack = mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    // Note: setting bounds is going to be redundant here
    // once kernel returns bounded capability.
    stack = cheri_bounds_set(stack, sz);
    stack = cheri_offset_set(stack, sz);
    stack = cheri_perms_and(stack, STACK_PERMS);


Then we can allocate some memory for the metadata, which contains a compartment descriptor and then we can create a 
capability pair.

.. code-block:: C
    
    void *data = mmap(NULL, pgsz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    cmpt_desc_t *desc = (cmpt_desc_t *)cheri_bounds_set_exact(data, sizeof(cmpt_desc_t));
    // Compartment descriptor:
    desc->stack = stack;
    desc->target = target;


And we have two ways to call the function in the compartment, and they're LB- and LPB-sealed 
capabilities.

LB-sealed Capabilities
----------------------
LB-sealed capabilities will leave unsealed copy in C29 because it uses instruction 
``B(L)R [C29, #off]`` to unseal C29 and branches to capability at  ``[C29,#off]``.

When using LB-sealed, we can simply fill in the relevant information in the compartment
descriptor.

.. code-block:: C

    desc->entry = cmpt_entry;
    desc->exit = cmpt_return;
    desc->stack = stack;
    if (!cheri_is_sealed(target)) {
        target = cheri_sentry_create(target);
    }
    desc->target = target;

    // Return read-only LB-sealed pointer to cap pair:
    return morello_lb_sentry_create(cheri_perms_and(desc, PERM_GLOBAL | READ_CAP_PERMS));

Also note that the ``cmpt_t`` type is defined as

.. code-block:: C

    /**
    * Compartment handle type (opaque).
    */
    typedef struct {
        void *data;
    } cmpt_t;

And where the ``morello_lb_sentry_create`` is a simple function with inline ASM that seals the cap pair.

.. code-block:: C

    /**
    * Create LB-sealed sentry.
    */
    inline static void * __capability morello_lb_sentry_create(void *cap)
    {
        void * __capability ret;
        __asm__ ("seal %0, %1, lb" : "=C"(ret) : "C"(cap));
        return ret;
    }

LPB-sealed Capabilities
-----------------------
In contrast to LB-sealed capabilities, LPB-sealed capabilities will unseal temporarily
to load and branch capability pair. It uses the instruction ``LDPB(L)R C29, [Cn]`` to
load ``C29<=[Cn]`` and branches to ``[Cn,#16]``. In this way, we can access through LPB
cap without exposing unsealed copy.

When using LPB-sealed cap, we should do

.. code-block:: C

    // Compartment descriptor:
    desc->stack = stack;
    desc->target = target;
    // Capability pair:
    cmpt_t *cmpt = (cmpt_t *)cheri_bounds_set_exact(data + sizeof(cmpt_desc_t), sizeof(cmpt_t));
    if (!cheri_is_sealed(target)) {
        target = cheri_sentry_create(target);
    }
    cmpt->data[0] = cheri_perms_and(desc, PERM_GLOBAL | READ_CAP_PERMS); // data capability
    cmpt->data[1] = cmpt_switch; // code capability
    // Return read-only LPB-sealed pointer to cap pair:
    return morello_lpb_sentry_create(cheri_perms_and(cmpt, PERM_GLOBAL | READ_CAP_PERMS));

and the ``cmpt_t`` type is defined as follows.

.. code-block:: C

    /**
    * Compartment handle type (opaque).
    */
    typedef struct {
        void *data[2];
    } cmpt_t;

And where the ``morello_lpb_sentry_create`` is a simple function with inline ASM that seals the cap pair.

.. code-block:: C

    /**
    * Create LB-sealed sentry.
    */
    inline static void * __capability morello_lpb_sentry_create(void *cap)
    {
        void * __capability ret;
        __asm__ ("seal %0, %1, lpb" : "=C"(ret) : "C"(cap));
        return ret;
    }

Besides these code, we also have three trampline functions in assembly code when using LB-sealed cap,
namely, ``cmpt_call``,  ``cmpt_entry`` and ``cmpt_return``. They're defined in 
`[lb.S] <https://git.morello-project.org/morello/morello-examples/-/blob/main/src/compartments/src/lb.S>`_ 
while there two trampline functions when using LPB-sealed cap, which are ``cmpt_call`` and ``cmpt_switch``,
and they're defined in `[lbp.S] <https://git.morello-project.org/morello/morello-examples/-/blob/main/src/compartments/src/lpb.S>`_

"Hello World" Example
---------------------

Now we can create a hello world example based on what we have above. You can find the full code here,
`[compartmentalisation-helloworld] <https://github.com/cocoa-xu/compartmentalisation-helloworld>`_ 

We only show the main part of this Hello World example at the end. To compile them, we can first clone 
the repo and then run ``gmake``. There will be two executables, ``hellolb`` and ``hellolpb``, and as the 
name suggests, they use LB- and LPB-sealed cap respectively. We can run them and see the output.

.. code-block:: Shell

    $ gmake
    cc -march=morello+c64 -mabi=purecap -Xclang -morello-vararg=new  -Iutil -DUSE_LB_SEALED_CAP util/capprint.c util/morello.c src/lb.S main.c -o hellolb
    cc -march=morello+c64 -mabi=purecap -Xclang -morello-vararg=new  -Iutil util/capprint.c util/morello.c src/lpb.S main.c -o hellolpb
    
    $ ./hellolb
    using LB-sealed capability
    before...
    csp: 0000fffffff7fe70 1 [0000ffffbff80000:0000fffffff80000) GrRMwWL-----I-V-23 none 1073741424 of 1073741824
    inside...
    csp: 0000000040a1ff40 1 [0000000040a1c000:0000000040a20000) GrRMwWL----------- none 16192 of 16384
    after...
    csp: 0000fffffff7fe70 1 [0000ffffbff80000:0000fffffff80000) GrRMwWL-----I-V-23 none 1073741424 of 1073741824
    result: 2 + 3 = 5
    
    $ ./hellolpb
    using LPB-sealed capability
    before...
    csp: 0000fffffff7fe70 1 [0000ffffbff80000:0000fffffff80000) GrRMwWL-----I-V-23 none 1073741424 of 1073741824
    inside...
    csp: 0000000040a1ff40 1 [0000000040a1c000:0000000040a20000) GrRMwWL----------- none 16192 of 16384
    after...
    csp: 0000fffffff7fe70 1 [0000ffffbff80000:0000fffffff80000) GrRMwWL-----I-V-23 none 1073741424 of 1073741824
    result: 2 + 3 = 5


.. code-block:: C

    #define _GNU_SOURCE

    #include <stdio.h>
    #include <stddef.h>
    #include <stdbool.h>
    #include <unistd.h>
    #include <sys/mman.h>

    #include "morello.h"

    /**
    * Wrappable function type.
    */
    typedef void *(cmpt_fun_t)(void* arg);

    #define STACK_PERMS (PERM_GLOBAL | READ_CAP_PERMS | WRITE_CAP_PERMS)

    #ifdef USE_LB_SEALED_CAP
    /**
    * LB-sealed
    * Compartment handle type (opaque).
    */
    typedef struct {
        void *data;
    } cmpt_t;

    /**
    * Compartment descriptor
    */
    typedef struct {
        void *entry;    // sentry for entry into compartment
        void *exit;     // sentry for return from compartment
        void *stack;    // callee's stack
        void *target;   // target function (sentry)
    } cmpt_desc_t;

    static const cmpt_t *create_cmpt(cmpt_fun_t *target, unsigned stack_pages)
    {
        size_t pgsz = getpagesize();
        size_t sz = pgsz * stack_pages;

        void *stack = mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        // Note: setting bounds is going to be redundant here
        // once kernel returns bounded capability.
        stack = cheri_bounds_set(stack, sz);
        stack = cheri_offset_set(stack, sz);
        stack = cheri_perms_and(stack, STACK_PERMS);

        void *data = mmap(NULL, pgsz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        cmpt_desc_t *desc = (cmpt_desc_t *)cheri_bounds_set_exact(data, sizeof(cmpt_desc_t));
        desc->entry = cmpt_entry;
        desc->exit = cmpt_return;
        desc->stack = stack;
        if (!cheri_is_sealed(target)) {
            target = cheri_sentry_create(target);
        }
        desc->target = target;

        // Return read-only LB-sealed pointer to cap pair:
        return morello_lb_sentry_create(cheri_perms_and(desc, PERM_GLOBAL | READ_CAP_PERMS));
    }

    // See src/lb.S
    // https://git.morello-project.org/morello/morello-examples/-/blob/main/src/compartments/src/lb.S
    extern void *cmpt_call(const cmpt_t *cmpt, void *arg);
    extern void cmpt_entry(void *arg);
    extern void cmpt_return();

    #else

    /**
    * LPB-sealed
    * Compartment handle type (opaque).
    */
    typedef struct {
        void *data[2];
    } cmpt_t;

    /**
    * Compartment descriptor
    */
    typedef struct {
        void *stack;
        void *target;
    } cmpt_desc_t;

    static const cmpt_t *create_cmpt(cmpt_fun_t *target, unsigned stack_pages)
    {
        size_t pgsz = getpagesize();
        size_t sz = pgsz * stack_pages;

        void *stack = mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        // Note: setting bounds is going to be redundant here
        // once kernel returns bounded capability.
        stack = cheri_bounds_set(stack, sz);
        stack = cheri_offset_set(stack, sz);
        stack = cheri_perms_and(stack, STACK_PERMS);

        void *data = mmap(NULL, pgsz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        cmpt_desc_t *desc = (cmpt_desc_t *)cheri_bounds_set_exact(data, sizeof(cmpt_desc_t));
        // Compartment descriptor:
        desc->stack = stack;
        desc->target = target;
        // Capability pair:
        cmpt_t *cmpt = (cmpt_t *)cheri_bounds_set_exact(data + sizeof(cmpt_desc_t), sizeof(cmpt_t));
        if (!cheri_is_sealed(target)) {
            target = cheri_sentry_create(target);
        }
        cmpt->data[0] = cheri_perms_and(desc, PERM_GLOBAL | READ_CAP_PERMS); // data capability
        cmpt->data[1] = cmpt_switch; // code capability
        // Return read-only LPB-sealed pointer to cap pair:
        return morello_lpb_sentry_create(cheri_perms_and(cmpt, PERM_GLOBAL | READ_CAP_PERMS));
    }

    // See src/lpb.S
    // https://git.morello-project.org/morello/morello-examples/-/blob/main/src/compartments/src/lpb.S
    extern void *cmpt_call(const cmpt_t *cmpt, void *arg);
    extern void *cmpt_switch(void *arg);

    #endif

    static void *fun(void *buffer)
    {
        printf("inside...\n");
        printf("csp: %s\n", cap_to_str(NULL, cheri_csp_get()));
        int *data = buffer;
        int x = data[0];
        int y = data[1];
        int z = x + y;
        data[2] = z;
        return data;
    }

    int main(int argc, char *argv[])
    {

    #ifdef USE_LB_SEALED_CAP
        printf("using LB-sealed capability\n");
    #else
        printf("using LPB-sealed capability\n");
    #endif

        const cmpt_t *cmpt = create_cmpt(fun, 4 /* pages */);
        int buffer[3] = {2, 3, 0};

        printf("before...\n");
        printf("csp: %s\n", cap_to_str(NULL, cheri_csp_get()));

        int *res = cmpt_call(cmpt, buffer);

        printf("after...\n");
        printf("csp: %s\n", cap_to_str(NULL, cheri_csp_get()));

        printf("result: %d + %d = %d\n", res[0], res[1], res[2]);
        return 0;
    }

