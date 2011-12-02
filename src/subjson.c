/* subjson
 * Parses a lighthearted subset of json into a subset of structures. I mean,
 * I'm not trying to embed it in javascript or anything...
 *
 * dict/object:
 *    { string  :  value ... }
 * list/array:
 *    [ value ... ]
 *
 * strings can be implied (not quoted), but if you want control characters, a
 * colon in the case of a dict or a space in the case of a list, you'll need to
 * quote them.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sysexits.h>
#include <err.h>


typedef struct {
    char      *filename;
    int        fd;
    ssize_t    size;

    char      *p_start;
    uint64_t  *p_quick;
    char      *p_curr;
    // ref_start for current "word" start...
    //
    // structures for holding the results objects...
} pstate;


pstate *init_from_file(char *filename) {
    ssize_t bytes_read;
    int     fd;
    struct  stat statbuf;
    pstate *state;

    if( (state = (pstate *) malloc(sizeof(pstate))) == NULL)
        err(EX_OSERR, "Couldn't allocate memory for parser state.");
    if( (fd = open(filename, O_RDONLY)) < 0)
        err(EX_NOINPUT, "Couldn't open %s.", filename);

    if( fstat(fd, &statbuf) == -1)
        err(EX_NOINPUT, "Opened, but couldn't stat %s.", filename);

    state->size     = statbuf.st_size;
    state->filename = filename;
    
    if( (state->p_start = (char *) malloc(state->size+2)) == NULL)
        err(EX_OSERR, "Couldn't allocate memory for file contents (%s).", filename);

    if( (bytes_read = read(fd, state->p_start, state->size)) != state->size)
        err(EX_DATAERR, "Only read %zd of %zd bytes from %s.", bytes_read, state->size, filename);

    state->p_curr   = state->p_start;
    state->p_quick  = (uint64_t *) state->p_start;
    state->p_curr[state->size] = 0; // Null terminate the whole thing just in case
    close(fd);
    return state;
}

void free_state(pstate *state) {
    if(state != NULL) {
        if(state->p_start != NULL) free(state->p_start);
        state->p_start = NULL;
        free(state);
    }
}


int parse(pstate *state) {

}


int main (int argc, char *argv[]) {
    pstate *state = init_from_file("../sjson-examples/example.sjson");
    parse(state);
    free_state(state);
}
