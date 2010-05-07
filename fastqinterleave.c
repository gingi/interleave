#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define DEBUG 1

typedef struct {
    char * name;
    char * seq;
    char * quality;
} sequence;

struct pe_sequence_t {
    struct pe_sequence_t * next;
    sequence * forward;
    sequence * reverse;
};

typedef struct pe_sequence_t pe_sequence;

char * readline(FILE * fh) {
    #define LINE_LEN 255
    char * str = (char *)malloc(LINE_LEN);
    fgets(str, LINE_LEN, fh);
    if (str[strlen(str) - 1] == '\n') {
        str[strlen(str) - 1] = '\0';
    }
    return str;
}

pe_sequence * llalloc(int initnext) {
    pe_sequence * seq = (pe_sequence *)malloc(sizeof(pe_sequence));
    if (initnext) {
        seq->next = (pe_sequence *)malloc(sizeof(pe_sequence));
    }
    return seq;
}

void llinsert(pe_sequence * seq, pe_sequence * target) {
    pe_sequence * rest = target->next;
    seq->next = rest;
    target->next = seq;
}

int is_forward(sequence * seq) {
    #define FORWARD '1'
    #define REVERSE '2'
    return seq->name[strlen(seq->name) - 1] == FORWARD;
}

int found_pair(const char * name, const pe_sequence * pair) {
    return memcmp(name, pair->forward->name, strlen(name) - 1) == 0;
}

void lazysearch(const char * name,
                pe_sequence ** match, 
                pe_sequence * root) {
    if ((*match)->next != NULL && found_pair(name, (*match)->next)) {
        (*match) = (*match)->next;
    } else {
        (*match) = root;
        while ((*match)->next != NULL && !found_pair(name, (*match)->next)) {
            (*match) = (*match)->next;
        }
    }
}

pe_sequence * load_fastq(const char * filename) {
    pe_sequence * root = llalloc(1);
    pe_sequence * curritem = root;
    pe_sequence * match = root;
    FILE * fh = fopen(filename, "r");
	if (fh == NULL) {
		printf("File %s cannot be read. Aborting.\n", filename);
		exit(1);
	}
#ifdef DEBUG
    fprintf(stderr, "Reading FASTQ file\n");
#endif
    do {
        char * name = readline(fh);
        char * seqstr = readline(fh);
        readline(fh);
        char * quality = readline(fh);
        sequence * seq = (sequence *)malloc(sizeof(sequence));
        pe_sequence * pair;
        name++;
        if (strlen(name) == 0) {
            break;
        }
        pair = llalloc(0);
        seq->name = strdup(name);
        seq->seq  = strdup(seqstr);
        seq->quality = strdup(quality);
#ifdef DEBUG
        fprintf(stderr, "Got seq %s\n", seq->name);
#endif
        if (is_forward(seq)) {
            pair->forward = seq;
            llinsert(pair, curritem);
            curritem = curritem->next;
        } else {
            lazysearch(seq->name, &match, root);
            if (match != NULL) {
                match->reverse = seq;
            }
        }
    } while (!feof(fh));
    fclose(fh);
#ifdef DEBUG
    fprintf(stderr, "Finished reading FASTQ\n");
#endif
    return root;
}

void printseq(FILE * fh, sequence * seq) {
    fprintf(fh, "@%s\n%s\n+%s\n%s\n",
        seq->name,
        seq->seq,
        seq->name,
        seq->quality
    );
}

void save_fastq(const char * filename, const pe_sequence * fastq) {
    pe_sequence * item = fastq->next;
	FILE * out = fopen(filename, "w");
	if (out == NULL) {
		printf("Cannot write to file %s\n", filename);
		exit(1);
	}
    while (item->next != NULL) {
        printseq(out, item->forward);
        printseq(out, item->reverse);
        item = item->next;
    }
	fclose(out);
}

int main (int argc, char const *argv[]) {
    pe_sequence * fastq;
    if (argc < 3) {
        printf("Usage: fastqinterleave <input.fastq> <output.fastq>\n");
        exit(0);
    }
    fastq = load_fastq(argv[1]);
    save_fastq(argv[2], fastq);
    return 0;
}

