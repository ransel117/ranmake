#define RAKE_IMPLEMENTATION
#include "rake.h"

int32_t main(int32_t argc, char *argv[]) {
    if (argc > 3) {
        RAKE_ERR_RET(RAKE_ERR, "too many arguments");
    } else if (argc > 1) {
        if (rake_str_eq(argv[1], RAKE_VER_ARG1) || rake_str_eq(argv[1], RAKE_VER_ARG2)) {
            return rake_print_ver();
        } else if (rake_str_eq(argv[1], RAKE_HELP_ARG1) || rake_str_eq(argv[1], RAKE_HELP_ARG2)) {
            return rake_print_help();
        } else if (rake_str_eq(argv[1], RAKE_F_ARG1) || rake_str_eq(argv[1], RAKE_F_ARG2)) {
            return rake_main(argv[2]);
        }
    }

    #if RAKE_DEBUG
    rake_print_args(argv, argc);
    #endif /* RAKE_DEBUG */

    return rake_main("");
}
