#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zlib.h"

#define LINE_LEN 255

#ifdef DEBUG
    #define dprint(format, ...) fprintf(stderr, format, ## __VA_ARGS__)
#else
    #define dprint(...) ((void)0)
#endif

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
    dprint("      R[%s]\n", str);
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
    dprint("Forward FH: %ld Reverse FH: %ld\n", ftell(ffh), ftell(rfh));
    dprint("Reading FASTQ file\n");
    do {
        sequence forward; 
        sequence reverse;
        readseq(ffh, &forward);
        readseq(rfh, &reverse);
        if (strlen(forward.name) == 0 || strlen(reverse.name) == 0) {
            dprint("Either forward or reverse is null. Just skipping.\n");
            continue;
        }
        if (!is_pair(&forward, &reverse)) {
            fprintf(stderr, "Mismatch of forward (%s) and reverse (%s) pairs. Aborting.\n", forward.name, reverse.name);
            exit(2);
        }
        dprint("Forward: [%s] Reverse: [%s]\n", forward.name, reverse.name);
        printseq(out, &forward);
        printseq(out, &reverse);
    } while (!feof(rfh));
    dprint("Finished reading FASTQ\n");
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

