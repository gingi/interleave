#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LINE_LEN 255

#ifdef DEBUG
    #define dprint(format, ...) fprintf(stderr, format, ## __VA_ARGS__)
#else
    #define dprint(...) ((void)0)
#endif

#define perr(format, ...) fprintf(stderr, format, ## __VA_ARGS__)

#define DEFAULT_LINES 4
int lines = DEFAULT_LINES;
char * outputfile;

typedef struct {
    char name[LINE_LEN];
    char seq[LINE_LEN];
    char quality[LINE_LEN];
} sequence;

void readline(FILE * fh, char * str) {
    fgets(str, LINE_LEN, fh);
    if (str[strlen(str) - 1] == '\n') {
        str[strlen(str) - 1] = '\0';
    }
}

int is_forward(sequence * seq) {
    #define FORWARD '1'
    #define REVERSE '2'
    return seq->name[strlen(seq->name) - 1] == FORWARD;
}

int is_pair(const sequence * forward, const sequence * reverse) {
    return
        memcmp(forward->name, reverse->name, strlen(forward->name) - 1) == 0;
}

void initseq(sequence * seq) {
    seq->name[0] = '\0';
    seq->seq[0] = '\0';
    seq->quality[0] = '\0';
}

void readseq(FILE * fh, sequence * seq) {
    initseq(seq);
    readline(fh, seq->name);
    if (strlen(seq->name) == 0) {
        return;
    }
    memmove(seq->name, seq->name + 1, strlen(seq->name) - 1);
    seq->name[strlen(seq->name) - 1] = '\0';
    readline(fh, seq->seq);
    char tmp[LINE_LEN];
    readline(fh, tmp);
    readline(fh, seq->quality);
}

void seekreverse(FILE * fh) {
    sequence seq;
    long prevpos;
    do {
        prevpos = ftell(fh);
        readseq(fh, &seq);
    } while (!feof(fh) && is_forward(&seq));
    fseek(fh, prevpos, SEEK_SET);
}

void printseq(FILE * fh, sequence * seq) {
    fprintf(fh, "@%s\n%s\n+%s\n%s\n",
        seq->name,
        seq->seq,
        seq->name,
        seq->quality
    );
}

void fopen_arr(FILE * files[], const char * filenames[], int num) {
    int i;
    for (i = 0; i < num; i++) {
        files[i] = fopen(filenames[i], "r");
        if (files[i] == NULL) {
            perr("File %s (%d) cannot be read. Aborting.\n", filenames[i], i);
            exit(1);
        }
    }
}

void fclose_arr(FILE * files[], int num) {
    int i;
    for (i = 0; i < num; i++) {
        fclose(files[i]);
    }
}

int feof_arr(FILE * files[], int num) {
    int i;
    for (i = 0; i < num; i++) {
        if (feof(files[i])) {
            return 1;
        }
    }
    return 0;
}

void interleave(int nfiles, const char * files[], int lines) {
    dprint("Interleave %d files\n", nfiles);
    dprint("Interleaving every %d lines\n", lines);
    FILE * in[nfiles];
    fopen_arr(in, files, nfiles);
    FILE * out = fopen(outputfile, "w");
    if (out == NULL) {
        perr("File %s cannot be written. Aborting.\n", outputfile);
        exit(1);
    }
    do {
        int i, j;
        char line[LINE_LEN];
        for (i = 0; i < nfiles; i++) {
            for (j = 0; j < lines; j++) {
                fgets(line, LINE_LEN, in[i]);
                if (line[strlen(line) - 1] != '\n') {
                    strcat(line, "\n");
                }
                fputs(line, out);
            }
        }
    } while (!feof_arr(in, nfiles));
    fclose_arr(in, nfiles);
    fclose(out);
}

void usage() {
    printf("Usage: interleave [args] <file1> <file2> ...\n\n");
    printf("args:\n");
    printf("   -l [num]   - Lines from each file to interleave (default: %d)\n", DEFAULT_LINES);
    printf("   -o [file]  - File to output\n");
    exit(0);
}

void parseopts(int * argc, char const **argv[]) {
    int ch;
    while ((ch = getopt(*argc, (char **)*argv, "l:o:")) != -1) {
        switch (ch) {
            case 'l': lines = atoi(optarg); break;
            case 'o': outputfile = optarg; break;
            default:  usage();
        }
    }
    *argc -= optind;
    *argv += optind;
}

int main (int argc, char const *argv[]) {
    parseopts(&argc, &argv);
    if (argc < 2) {
        usage();
    }
    interleave(argc, argv, lines);
    return 0;
}

