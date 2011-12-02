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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>


typedef struct {
    char      *filename;
    size_t     size;

    char      *p_start;
    uint64_t  *p_quick;
    char      *p_curr;
    // ref_start for current "word" start...
    //
    // structures for holding the results objects...
} pstate;


int read_in_file(char *filename) {
    struct stat sb;


    fd = open(argv[1], O_RDONLY);    if(fd == -1) {perror("open");  return 1;}
    if(fstat(fd, &sb) == -1)                      {perror("fstat"); return 1;}
    if(!S_ISREG (sb.st_mode)) {
        fprintf (stderr, "%s is not a file\n", filename);
        return 1;
    }
    // TODO: figure out exact size...

    // TODO: allocate and initialize a new pstate

    //p = mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    //if (p == MAP_FAILED) { perror ("mmap"); return 1; }

}


int main (int argc, char *argv[]) {
    struct stat sb;
    off_t len;
    char *p;
    int fd;

    //if (argc < 2) { fprintf (stderr, "usage: %s <file>\n", argv[0]); return 1; }

    fd = open(argv[1], O_RDONLY);
    fstat(fd, &sb);
    p = mmap(0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    close(fd);
    for (len = 0; len < sb.st_size; len++) putchar (p[len]);
    munmap(p, sb.st_size);

    //if (fd == -1) { perror ("open"); return 1; }

    //if (fstat (fd, &sb) == -1) { perror ("fstat"); return 1; }

    //if (!S_ISREG (sb.st_mode)) { fprintf (stderr, "%s is not a file\n", argv[1]); return 1; }

    //p = mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    //if (p == MAP_FAILED) { perror ("mmap"); return 1; }

    //if (close (fd) == -1) { perror ("close"); return 1; }

    //for (len = 0; len < sb.st_size; len++)
        //putchar (p[len]);

    //if (munmap (p, sb.st_size) == -1) { perror ("munmap"); return 1; }
    //return 0;
}


int main(int argc, char *argv[]) {

}
