#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zlib.h"

// #define DEBUG

#ifdef DEBUG
    #define DPRINT(format, ...) fprintf(stderr, format, ## __VA_ARGS__)
#else
    #define DPRINT(...) ((void)0)
#endif

typedef struct {
    char * name;
    char * seq;
    char * quality;
} sequence;

char * readline(FILE * fh) {
    #define LINE_LEN 255
    char line[LINE_LEN] = "\0";
    fgets(line, LINE_LEN, fh);
    if (line[strlen(line) - 1] == '\n') {
        line[strlen(line) - 1] = '\0';
    }
    char * str = (char *)malloc(strlen(line) + 1);
    memmove(str, line, strlen(line));
    return str;
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

sequence * readseq(FILE * fh) {
    DPRINT("Reading next seq...\n");
    sequence * seq = (sequence *)malloc(sizeof(sequence));
    char * name = readline(fh);
    if (strlen(name) == 0) {
        return NULL;
    }
    seq->name = (char *)malloc(strlen(name));
    memmove(seq->name, name + 1, strlen(name));
    seq->seq     = readline(fh);
    readline(fh);
    seq->quality = readline(fh);
    return seq;    
}

void seekreverse(FILE * fh) {
    sequence * seq;
    long prevpos;
    do {
        prevpos = ftell(fh);
        DPRINT("BEFORE READ: %ld\n", prevpos);
        seq = readseq(fh);
    } while (!feof(fh) && is_forward(seq));
    if (seq != NULL) {
        fseek(fh, prevpos, SEEK_SET);
    }
}

void printseq(FILE * fh, sequence * seq) {
    fprintf(fh, "@%s\n%s\n+%s\n%s\n",
        seq->name,
        seq->seq,
        seq->name,
        seq->quality
    );
}

void fastqinterleave(const char * infile, const char * outfile) {
    FILE * ffh = fopen(infile, "r");
    FILE * rfh = fopen(infile, "r");
    if (ffh == NULL || rfh == NULL) {
        fprintf(stderr, "File %s cannot be read. Aborting.\n", infile);
        exit(1);
    }
    FILE * out = fopen(outfile, "w");
    if (out == NULL) {
        fprintf(stderr, "File %s cannot be written. Aborting.\n", outfile);
        exit(1);
    }
    seekreverse(rfh); // Advance to position of first REVERSE mate
    DPRINT("FORWARD FH: %ld REVERSE FH: %ld\n", ftell(ffh), ftell(rfh));
    DPRINT("Reading FASTQ file\n");
    do {
        sequence * forward = readseq(ffh);
        sequence * reverse = readseq(rfh);
        if (forward == NULL || reverse == NULL) {
            DPRINT("Either forward or reverse is null. Just skipping.\n");
            continue;
        }
        if (!is_pair(forward, reverse)) {
            fprintf(stderr, "Mismatch of forward (%s) and reverse  (%s) pairs. Aborting.\n", forward->name, reverse->name);
            exit(2);
        }
        DPRINT("FORWARD [%s] REVERSE [%s]\n", forward->name, reverse->name);
        printseq(out, forward);
        printseq(out, reverse);
    } while (!feof(rfh));
    DPRINT("Finished reading FASTQ\n");
    fclose(ffh);
    fclose(rfh);
    fclose(out);
}

int main (int argc, char const *argv[]) {
    if (argc < 3) {
        printf("Usage: fastqinterleave <input.fastq> <output.fastq>\n");
        exit(0);
    }
    fastqinterleave(argv[1], argv[2]);
    return 0;
}

