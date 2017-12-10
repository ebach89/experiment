#include <stdio.h>

#include "library.h"

#define PROG_TAG    "libfoo: "

void greeting(void)
{
    printf(PROG_TAG "hello world\n");
}

void to_be_hooked(void)
{
    printf(PROG_TAG "Hi, I'm %s()\n", __func__);
}
