/* http://man7.org/linux/man-pages/man7/rtld-audit.7.html */
#include <stdio.h>
#define __USE_GNU
#include <dlfcn.h>  /* for Lmid_t */

#define _GNU_SOURCE
#include <link.h>

#include <libgen.h>
#include <string.h>

#define _unused_         __attribute__((unused))

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
#include "rtld_audit_symbind_common.c"
#undef __la_symbind_XXX
#undef ElfXXX_Sym

#define  __la_symbind_XXX       __la_symbind_64
#define ElfXXX_Sym              Elf64_Sym
#include "rtld_audit_symbind_common.c"
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

/* from bits/link.h */
#ifdef __x86_64__

#define __la_XXX_gnu_pltenter_impl      __la_x86_64_gnu_pltenter
#define __la_XXX_gnu_pltexit_impl       __la_x86_64_gnu_pltexit
#define ElfXXX_Addr                     Elf64_Addr
#define ElfXXX_Sym                      Elf64_Sym
#define La_XXX_regs                     La_x86_64_regs
#define La_XXX_retval                   La_x86_64_retval
#include "rtld_audit_plt_common.c"
#undef __la_XXX_gnu_pltenter_impl
#undef __la_XXX_gnu_pltexit_impl
#undef ElfXXX_Addr
#undef ElfXXX_Sym
#undef La_XXX_regs
#undef La_XXX_retval

Elf64_Addr la_x86_64_gnu_pltenter(Elf64_Sym *sym,
                                  unsigned int ndx,
                                  uintptr_t *refcook,
                                  uintptr_t *defcook,
                                  La_x86_64_regs *regs,
                                  unsigned int *flags,
                                  const char *symname,
                                  long int *framesizep)
{
    return __la_x86_64_gnu_pltenter(sym, ndx, refcook, defcook, regs, flags, symname, framesizep);
}

unsigned int la_x86_64_gnu_pltexit(Elf64_Sym *sym,
                                   unsigned int ndx,
                                   uintptr_t *refcook,
                                   uintptr_t *defcook,
                                   const La_x86_64_regs *inregs,
                                   La_x86_64_retval *outregs,
                                   const char *symname)
{
    return __la_x86_64_gnu_pltexit(sym, ndx, refcook, defcook, inregs, outregs, symname);
}

#define __la_XXX_gnu_pltenter_impl      __la_x32_gnu_pltenter
#define __la_XXX_gnu_pltexit_impl       __la_x32_gnu_pltexit
#define ElfXXX_Addr                     Elf32_Addr
#define ElfXXX_Sym                      Elf32_Sym
#define La_XXX_regs                     La_x32_regs
#define La_XXX_retval                   La_x32_retval
#include "rtld_audit_plt_common.c"
#undef __la_XXX_gnu_pltenter_impl
#undef __la_XXX_gnu_pltexit_impl
#undef ElfXXX_Addr
#undef ElfXXX_Sym
#undef La_XXX_regs
#undef La_XXX_retval

Elf32_Addr la_x32_gnu_pltenter(Elf32_Sym *sym,
                               unsigned int ndx,
                               uintptr_t *refcook,
                               uintptr_t *defcook,
                               La_x32_regs *regs,
                               unsigned int *flags,
                               const char *symname,
                               long int *framesizep)
{
    return __la_x32_gnu_pltenter(sym, ndx, refcook, defcook, regs, flags, symname, framesizep);
}

unsigned int la_x32_gnu_pltexit(Elf32_Sym *sym,
                                unsigned int ndx,
                                uintptr_t *refcook,
                                uintptr_t *defcook,
                                const La_x32_regs *inregs,
                                La_x32_retval *outregs,
                                const char *symname)
{
    return __la_x32_gnu_pltexit(sym, ndx, refcook, defcook, inregs, outregs, symname);
}
#else /* !__x86_64__ */

Elf32_Addr la_i86_gnu_pltenter(Elf32_Sym *sym,
                               unsigned int ndx,
                               uintptr_t *refcook,
                               uintptr_t *defcook,
                               La_i86_regs *regs,
                               unsigned int *flags,
                               const char *symname,
                               long int *framesizep);


unsigned int la_i86_gnu_pltexit(Elf32_Sym *sym,
                                unsigned int ndx,
                                uintptr_t *refcook,
                                uintptr_t *defcook,
                                const La_i86_regs *inregs,
                                La_i86_retval *outregs,
                                const char *symname);

#endif /* __x86_64__ */
