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

#define __la_symbind_XXX        __la_symbind_32
#define ElfXXX_Sym              Elf32_Sym
#include "rtld_audit_common.c"
#undef __la_symbind_XXX
#undef ElfXXX_Sym

#define  __la_symbind_XXX       __la_symbind_64
#define ElfXXX_Sym              Elf64_Sym
#include "rtld_audit_common.c"
#undef __la_symbind_XXX
#undef ElfXXX_Sym

uintptr_t la_symbind32(Elf32_Sym *sym, unsigned int ndx,
                       uintptr_t *refcook, uintptr_t *defcook,
                       unsigned int *flags, const char *symname)
{
    return __la_symbind_32(sym, ndx, refcook, defcook, flags, symname);
}

uintptr_t la_symbind64(Elf64_Sym *sym, unsigned int ndx,
                       uintptr_t *refcook, uintptr_t *defcook,
                       unsigned int *flags, const char *symname)
{
    return __la_symbind_64(sym, ndx, refcook, defcook, flags, symname);
}
