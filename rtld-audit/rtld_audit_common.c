
static uintptr_t __la_symbind_XXX(ElfXXX_Sym *sym,
                                  unsigned int ndx,
                                  uintptr_t *refcook,
                                  uintptr_t *defcook,
                                  unsigned int *flags,
                                  const char *symname)
{
    printf(PROG_TAG "%s(): symname = %s; sym->st_value = %p\n"
           "        ndx = %d; flags = 0x%x"
           "; refcook = %p; defcook = %p\n",
           __func__, symname, (void *)(uintptr_t)sym->st_value,
           ndx, *flags,
           refcook, defcook);

    if (!strcmp(symname, STRFY(greeting))) {
        greeting();
        return sym->st_value;
    }
    if (!strcmp(symname, "to_be_hooked")) {
        return (uintptr_t)hook_for_to_be_hooked;
    }
    return sym->st_value;
}

