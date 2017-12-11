#include <stdio.h>

#include "library.h"

#define PROG_TAG    "APP: "

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    printf("Test-case #1: how binding works - la_symbind32/64() is called once\n");
    printf(PROG_TAG "Attempt #1: Going to call greeting() from libfoo.so\n");
    greeting();
    printf(PROG_TAG "Attempt #2: Going to call greeting() from libfoo.so\n");
    greeting();


    printf("\nTest-case #2: Intercept functions from libfoo.so by Auditing library\n");
    printf(PROG_TAG "Attempt #1: Going to call to_be_hooked() from libfoo.so\n");
    to_be_hooked();
    printf(PROG_TAG "Attempt #2: Going to call to_be_hooked() from libfoo.so\n");
    to_be_hooked();
    return 0;
}
