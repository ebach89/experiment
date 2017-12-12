static ElfXXX_Addr __la_XXX_gnu_pltenter_impl(ElfXXX_Sym *sym _unused_,
                                              unsigned int ndx _unused_,
                                              uintptr_t *refcook _unused_,
                                              uintptr_t *defcook _unused_,
                                              La_XXX_regs *regs _unused_,
                                              unsigned int *flags _unused_,
                                              const char *symname _unused_,
                                              long int *framesizep _unused_)
{
    printf(PROG_TAG "%s(): symname:%s (sym->st_value:%p)\n",
           __func__, symname, (void *)(uintptr_t)sym->st_value);

    return sym->st_value;
}
