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

#define UNPACK_STATE()       uint64_t *qcurr = state->p_quick; \
                             char     *curr  = state->p_curr;  \
                             uint64_t *qeof  = state->p_qend;  \
                             char     *eof   = state->p_end;
#define COMMIT_STATE()       state->p_quick = qcurr; state->p_curr = curr
#define discard() curr++; COMMIT_STATE();

// Modified from http://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
                         //  (v - 0x0101...) & ~v & 0x8080...
#define q_haszero(v)         ((v) - UINT64_C(0x0101010101010101)) & ~(v) & UINT64_C(0x8080808080808080)
#define q_hasval(v,n)        (q_haszero((v) ^ (~UINT64_C(0)/255 * (n))))

// Search for one of two characters- first big chunks at a time and then get
// the actual byte. (First statement resets qcurr to whatever curr is so curr
// is authoritative).
#define qscan2(c1,c2)        qcurr=(uint64_t *)curr; \
                             while((qcurr <= qeof) && !q_hasval(*qcurr,(c1)) && !q_hasval(*qcurr,(c2)))\
                                 qcurr++; \
                             curr=(char *)qcurr;     \
                             while((curr <= eof) && (*curr != (c1)) && (*curr != (c2)))\
                                 curr++;
#define qscan3(c1,c2,c3)     qcurr=(uint64_t *)curr; \
                             while((qcurr <= qeof) && !q_hasval(*qcurr,(c1)) && !q_hasval(*qcurr,(c2)) && !q_hasval(*qcurr,(c3)))\
                                 qcurr++; \
                             curr=(char *)qcurr;     \
                             while((curr <= eof) && (*curr != (c1)) && (*curr != (c2)) && (*curr != (c3)))\
                                 curr++;

// { 0x7b 
// [ 0x5b
// # 0x23


typedef struct {
    char      *filename;
    int        fd;
    ssize_t    size;
    ssize_t    qsize;

    char      *p_start;
    uint64_t  *p_quick;
    char      *p_curr;

    char      *p_end;
    uint64_t  *p_qend;
    // ref_start for current "word" start...
    //
    // structures for holding the results objects...
    //
    // a 0111 1011
    // b 0101 1011
    // c 0010 0011
    //   ---------
    //   0222 2033
    //
    // if(!(n & 0000,0011 ^ 1111,1100)) {
    //   if(
    //
} pstate;

void reset_state(pstate *state) {
    state->p_curr   = state->p_start;
    state->p_quick  = (uint64_t *) state->p_start;
}

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
    state->qsize    = state->size >> 3; // size in uint64_t chunks
    state->filename = filename;
    
    // padding to the right so that quickscan stuff can look in bigger chunks
    if( (state->p_start = (char *) malloc(state->size+8)) == NULL)
        err(EX_OSERR, "Couldn't allocate memory for file contents (%s).", filename);

    if( (bytes_read = read(fd, state->p_start, state->size)) != state->size)
        err(EX_DATAERR, "Only read %zd of %zd bytes from %s.", bytes_read, state->size, filename);

    reset_state(state);
    state->p_end    = &(state->p_start[state->size - 1]);
    state->p_qend   = &(state->p_quick[state->qsize - 1]);
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
    UNPACK_STATE();
    return toplevel(state);
}

int toplevel(pstate *state) {
    UNPACK_STATE();
    int found = 0;
    while(curr <= eof) {
        qscan3('{','[','#');
        switch(*curr) {
            case '{': discard(); found += new_dict(state); break;
            case '[': discard(); found += new_list(state); break;
            case '#': discard(); found ++; break;
            default:  discard(); // EOF
        }
    }
    return found;
}

int new_dict(pstate *state) {

    //printf("\n-->DICT\n%s\n", state->p_curr);
    return 1;
}

int new_list(pstate *state) {
    //printf("\n-->LIST\n%s\n", state->p_curr);
    // - create structure
    // - next_value* until ']'
    return 1;
}

int next_value(pstate *state) {
    // - skipwhites
    // - could be: explicit-string, list, dict, scalar, nothing(comma),
    //             nothing(end-bracket)
    UNPACK_STATE();
    //skipwhites();

}


int main (int argc, char *argv[]) {
    int i;
    int found = 0;
    pstate *state = init_from_file("../sjson-examples/big.txt");
    for(i=0; i<10000; i++) {
        found += parse(state);
        reset_state(state);
    }
    free_state(state);
    printf("%d\n", found);
}
