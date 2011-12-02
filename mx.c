/*
 * 
 *
 */

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define streq(s1, s2) (strcmp((s1),(s2)) == 0)

#define PATHSIZE 1024
#define MCDIR "/.mx"
#define MCDIR_L 9

char root_dir[PATHSIZE];

void usage() {
    printf("USAGE: Not really usable yet\n");
}

void error(const char* msg, ...) {
    va_list ap;
    va_start(ap, msg);
    fprintf(stderr, "ERROR: ");
    vfprintf(stderr, msg, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}

int find_root_dir() {
    getcwd(root_dir, PATHSIZE - MCDIR_L - 3);
    if(root_dir == NULL) {
        error("Couldn't open current directory");
        exit(3);
    }
    int endpos = strlen(root_dir);
    do {
        memcpy(&root_dir[endpos], MCDIR, MCDIR_L+1);
        if(access(root_dir, X_OK) == 0) return 1;
    } while( (endpos = go_up(root_dir)) );
    return 0;
}

int go_up(char *dir) {
    int i = 0;
    int last_pos = 0;
    int last_pos2 = 0;
    char ch;
    while((ch = dir[i])) {
        if(ch == '/' || ch == '\\') {
            if(dir[i+1]) {
                last_pos2 = last_pos;
                last_pos = i; // don't count if trailing slash
            }
        }
        i++;
    }
    return last_pos2;
}

void curr() {
    // 1. project name (directory or .mx/ override)
    // 2. git context/state (with state flags like merging/cherry-picking/unmerged...)
    // 3. environment
    // 4. role
    // 5. machine (host)
    // 6. os
    
    stat_project
    printf("/");
    stat_context(1);
    printf("/");
    stat_role(1);
}

int main(int argc, char *argv[], char *envp[]) {
    if(argc < 2) {
        usage();
        return 1;
    }
    /* -- Commands --
     *                                    => stat + help
     * (help|-h|--help|-help)             => help
     * (version|-v|--version|-version)    => version
     *
     * init [directory]                   => init
     * st[at] [-verbose] [-long] [type]   => stat  `mx st env` == `mx env`
     *
     * role [-list|new]                   => role_ops
     * env[ironment] [-list|new]          => env_ops
     * os                                 => os_ops
     * host                               => host_ops
     *
     * conf[ig] [group]                   => conf
     * refresh                            => refresh project based on current stat
     *
     * [COMMAND]                          => misc. command from .mx/commands/...
     *
     * -- Environment variables / settings --
     *
     * environment  |  settings  |  default
     * -------------+------------+-----------
     *  MX_COLOR    | mx-color   |    1
     *  MX_PATH     | mx-path    | 
     *
     */

    if(streq(argv[1], "help")) {
        usage(); return 0;
    } else if(streq(argv[1], "curr")) {
        if(find_root_dir()) {
            curr();
        } else {
            error("Not in a mx project.");
        }
    } else error("Unknown command %s. Try 'help'.", argv[1]);
}
