/* http://man7.org/linux/man-pages/man7/rtld-audit.7.html */
#include <stdio.h>
#define __USE_GNU
#include <dlfcn.h>  /* for Lmid_t */

#define _GNU_SOURCE
#include <link.h>

#include <libgen.h>
#include <string.h>


#define PROG_TAG    "AUDIT: "

const char *audited_libs[] = {
    ".", /* It seems, it means Application. without registering it at la_objopen() 
          * the auditing notifications for bindings into the foo.so does not work */
    "foo.so",
    NULL
};

#define ARRAY_SIZE(a)   sizeof(a)/sizeof(a[0])
#define STRFY(x)    #x

void greeting(void)
{
    printf(PROG_TAG "hello world\n");
}

void hook_for_to_be_hooked(void)
{
    printf(PROG_TAG "Hi, I'm %s()\n", __func__);
}

unsigned int la_version(unsigned int version)
{
    printf(PROG_TAG "la_version(): %d\n", version);

    return version;
}

unsigned int la_objopen(struct link_map *map, Lmid_t lmid, uintptr_t *cookie)
{
    char *base_name = basename(map->l_name);
    unsigned int flags = 0;
    size_t i;

    printf(PROG_TAG "la_objopen(): loading \"%s\" (\"%s\"); lmid = %s; cookie=%p\n",
           map->l_name, base_name,
           (lmid == LM_ID_BASE) ?  "LM_ID_BASE" :
           (lmid == LM_ID_NEWLM) ? "LM_ID_NEWLM" : "???",
           cookie);

    for (i = 0; audited_libs[i] != NULL; ++i)
    {
        if (!strcmp(base_name, audited_libs[i])) {
            printf(PROG_TAG "la_objopen(): going to auditing: %s\n", base_name);
            flags = LA_FLG_BINDTO | LA_FLG_BINDFROM;
            break;
        }
    }
    return flags;
}

uintptr_t la_symbind32(Elf32_Sym *sym, unsigned int ndx,
                       uintptr_t *refcook, uintptr_t *defcook,
                       unsigned int *flags, const char *symname)
{
    printf(PROG_TAG "la_symbind32(): symname = %s; sym->st_value = %p\n"
           "        ndx = %d; flags = 0x%x"
           "; refcook = %p; defcook = %p\n",
            symname, (void *)sym->st_value,
            ndx, *flags,
            refcook, defcook);

    if (!strcmp(symname, STRFY(greeting))) {
        greeting();
        return sym->st_value;
    }
    if (!strcmp(symname, "to_be_hooked")) {
        return (uintptr_t)hook_for_to_be_hooked;
    } 
}

