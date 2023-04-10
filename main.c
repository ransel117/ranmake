#define RANMAKE_IMPLEMENTATION
#include "ranmake.h"

int32_t main(int32_t argc, char *argv[]) {
    if (argc > 3) {
        RANMAKE_ERR_RET(RANMAKE_ERR, "too many arguments");
    } else if (argc > 1) {
        if (ranmake_str_eq(argv[1], RANMAKE_VER_ARG1) || ranmake_str_eq(argv[1], RANMAKE_VER_ARG2)) {
            return ranmake_print_ver();
        } else if (ranmake_str_eq(argv[1], RANMAKE_HELP_ARG1) || ranmake_str_eq(argv[1], RANMAKE_HELP_ARG2)) {
            return ranmake_print_help();
        } else if (ranmake_str_eq(argv[1], RANMAKE_F_ARG1) || ranmake_str_eq(argv[1], RANMAKE_F_ARG2)) {
            return ranmake_main(argv[2]);
        }
    }

    #if RANMAKE_DEBUG
    ranmake_print_args(argv, argc);
    #endif /* RANMAKE_DEBUG */

    return ranmake_main("");
}
