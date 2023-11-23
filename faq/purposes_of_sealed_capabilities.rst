=============================================
What are the purposes of sealed capabilities?
=============================================

Sealed capabilities are a mechanism to allow a capability to be passed
between processes without the receiving process being able to forge a
capability to the same object. They serve several purposes related to 
security and capability-based systems:

#. Tamper-Proofing

  Sealed capabilities help protect against tampering or unauthorized 
  modification of capabilities. Once a capability is sealed, its properties,
  such as permissions and bounds, are fixed, and any attempt to tamper with
  the capability will result in an invalid capability, which we can validate
  it using ``cheri_is_valid``, or a trap will be raised once the program
  attempts to use the invalid capability. Please see the post
  `How do I seal a capability? <https://capabilitiesforcoders.com/faq/how_to_seal.html>`
  for more details.

#. Code and Control Flow Integrity

  Sealed capabilities are often used to ensure the integrity of code. By sealing
  a code capability, you can prevent the code from being modified or replaced
  with malicious instructions. Also, it prevents attackers to jump to arbitrary
  locations in the code.

  One example is to mitigate return-oriented programming (ROP) attacks. ROP attacks
  typically involve chaining together small snippets of existing code, known as 
  "gadgets", to execute arbitrary code. CHERI can prevent ROP attacks by sealing
  automatically sealing function pointers (i.e., creating a sentry) and the return
  address (link register).

#. Memory Safety

  Sealed capabilities can be used to ensure memory safety. This can help prevent
  common programming errors like buffer overflows.

  .. code-block:: c

    #include <stdio.h>
    #include <stdlib.h>

    int main()
    {
      int i = 0;
      char buf[4] = {'\0'};
      printf("Please input a three letter word: ");
      fflush(stdout);
      scanf("%s", buf);
      printf("buf: %#p, %s\n", buf, buf);
      printf("i: %d\n", i);
    }


  And if we compile and run the program, and type ``AAAAA`` when it prompts for input,
  we will see the following output on a CHERI system:

  .. code-block:: shell

    $ clang -march=morello \
        --target=aarch64-linux-musl_purecap \
        --sysroot=/root/musl-sysroot-purecap \
        memory-safety.c -o memory-safety -static
    $ morelloie -- ./memory-safety
    Please input a three letter word: AAAAA
    [emulator] simulated capability fault at 2167cc in thread 732
    Out of bounds access to 1 bytes at ffffd8176f8c
    0x1:dc104000:6f8c6f88:0000ffff:d8176f8c
              tag: true
          address: 0x00000ffffd8176f8c
             base: 0x00000ffffd8176f88
            limit: 0x00000ffffd8176f8c
           bounds: valid
        in bounds: false
           length: 4
           offset: 4
      permissions: GrRMwWL-----------
           sealed: (not sealed)
            flags: 0
         exponent: 0
              top: 0x6f8c
           bottom: 0x6f88
    Segmentation fault

  However, if we run the same program on a non-CHERI system, we will see something
  similar to the following output:

  .. code-block:: shell

    $ clang -Wno-format memory-safety.c -o memory-safety
    $ ./memory-safety
    Please input a three letter word: AAAAA
    buf: 0x16f16b2e8, AAAAA
    i: 65

  As we can see, the program on a non-CHERI system does not crash, but it prints
  out the value of ``i`` as ``65``, which is the ASCII value of ``A``. This is
  because the program overwrote the value of ``i`` with the value of ``buf``.

#. Fine-Grained Access Control

  Sealed capabilities can be used to implement fine-grained access control. For
  example, you can seal a capability to a specific memory region, and pass it to
  another process. The receiving process can only access the memory region that
  the capability points to, and cannot access any other memory regions.

  .. code-block:: c

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <cheriintrin.h>

    #ifdef __CHERI
    #define PTR_FORMAT "%#p"
    #else
    #define PTR_FORMAT "%p"
    #endif

    void untrusted_3rd_party_func(char* buf);

    int main()
    {
      char * message = "<secret part | public part>";
      char * public_part = strchr(message, '|') + 2;
    #ifdef __CHERI
      public_part = cheri_bounds_set(public_part, strlen(public_part) + 1);
    #endif
      printf("[main] message    : " PTR_FORMAT ", %s\n", message, message);
      printf("[main] public_part: " PTR_FORMAT ", %s\n", public_part, public_part);
      untrusted_3rd_party_func(public_part);
    }

    void untrusted_3rd_party_func(char* public_part)
    {
      printf("[3rd_party_func] public_part: " PTR_FORMAT ", %s\n", public_part, public_part);
      char * secret_part = public_part;
      while (*secret_part != '<') {
        secret_part--;
      }
      printf("[3rd_party_func] secret_part: " PTR_FORMAT ", %s\n", secret_part, secret_part);
    }


  And if we compile and run the program, we will see the following output on a CHERI system:

  .. code-block:: shell

    $ clang -march=morello \
        -D__CHERI \
        --target=aarch64-linux-musl_purecap \
        --sysroot=/root/musl-sysroot-purecap \
        access_control.c -o access_control -static
    $ morelloie -- ./access_control
    [main] message    : 0x2002cf [rR,0x2002cf-0x2002eb], <secret part | public part>
    [main] public_part: 0x2002de [rR,0x2002de-0x2002eb], public part>
    [3rd_party_func] public_part: 0x2002de [rR,0x2002de-0x2002eb], public part>
    [emulator] simulated capability fault at 2116d4 in thread 973
    Out of bounds access to 1 bytes at 0000002002dd
    0x1:90104000:42eb02de:00000000:002002dd
              tag: true
          address: 0x000000000002002dd
             base: 0x000000000002002de
            limit: 0x000000000002002eb
           bounds: valid
        in bounds: false
           length: 13
           offset: -1
      permissions: GrRM--------------
           sealed: (not sealed)
            flags: 0
         exponent: 0
              top: 0x02eb
           bottom: 0x02de
    Segmentation fault


  As we can see, the program crashes when the third-party function tries to access
  the secret part of the message.

