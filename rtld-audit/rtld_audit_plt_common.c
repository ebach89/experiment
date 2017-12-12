static ElfXXX_Addr __la_XXX_gnu_pltenter_impl(ElfXXX_Sym *sym _unused_,
                                              unsigned int ndx _unused_,
                                              uintptr_t *refcook _unused_,
                                              uintptr_t *defcook _unused_,
                                              La_XXX_regs *regs _unused_,
                                              unsigned int *flags _unused_,
                                              const char *symname _unused_,
                                              long int *framesizep _unused_)
{
    printf(PROG_TAG "%s(): Enter PLT before call symname:%s() (sym->st_value:%p)\n",
           __func__, symname, (void *)(uintptr_t)sym->st_value);

    /* without properly set of framesizep, the pltexit() will not be called */
    *framesizep = 256;
    return sym->st_value;
}

static unsigned int __la_XXX_gnu_pltexit_impl(ElfXXX_Sym *sym _unused_,
                                              unsigned int ndx _unused_,
                                              uintptr_t *refcook _unused_,
                                              uintptr_t *defcook _unused_,
                                              const La_XXX_regs *inregs _unused_,
                                              La_XXX_retval *outregs _unused_,
                                              const char *symname _unused_)
{
    printf(PROG_TAG "%s(): Exit PLT after call symname:%s() (sym->st_value:%p)\n",
           __func__, symname, (void *)(uintptr_t)sym->st_value);

    return 0;
}
