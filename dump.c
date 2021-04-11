#include "../shell.h"
#include "../symbtable/symboltable.h"

int dump(int argc, char **argv)
{
    dump_local_symtab();
    return 0;
}
