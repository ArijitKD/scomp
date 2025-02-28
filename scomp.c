#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NULLPAD(datlen)        datlen+1
#define MIN_ARGS_FOR_UNPACK    5
#define FIRST_DSEQ_ELEM_INDEX  4
#define UNPACK_STRING_INDEX    2
#define DSEQ_ARG_INDEX         3
#define UNPACK_INDEX_IF_USED   1
#define MAX_INDEX_NO_UNPACK    2
#define NO_UNPACK_STR_INDEX    1

struct compressed_data {
        char* data;
        size_t* decode_seq;
};

struct compressed_data* compress(char* data)
{
        if (!data)
                return NULL;

        struct compressed_data* cd = malloc(sizeof(*cd));
        if (!cd)
                return NULL;
        
        size_t datlen  = strlen(data);
        cd->data       = calloc(NULLPAD(datlen), sizeof(char));
        cd->decode_seq = malloc(datlen * sizeof(size_t));
        
        if (!cd->decode_seq || !cd->data) {
                free(cd->decode_seq);
                free(cd->data);
                free(cd);
                return NULL;
        }
        
        size_t i = 0, j = 0, count;
        while (i < datlen) {
                count = 0;
                
                for (char ch = data[i]; ch == data[i]; ++i)
                        ++count;
                        
                cd->data[j]       = data[i-1];
                cd->decode_seq[j] = count;
                ++j; 
        }
        
        // Now try to free some memory
        ++j;
        char* tempdata   = realloc(cd->data, j * sizeof(char));
        --j;
        size_t* tempdseq = realloc(cd->decode_seq, j * sizeof(size_t));
        
        if (tempdata)
                cd->data = tempdata;

        if (tempdseq)
                cd->decode_seq = tempdseq;

        return cd;   
}

char* decompress(struct compressed_data* cd)
{
        if (!cd || !cd->data || !cd->decode_seq)
                return NULL;

        size_t compresslen = strlen(cd->data);
        size_t datlen = 0;
        
        for (size_t i = 0; i < compresslen; ++i)
                datlen += cd->decode_seq[i];

        char* unpacked_data = calloc(NULLPAD(datlen), sizeof(char));
        
        if (!unpacked_data)
                return NULL;

        size_t i = 0, j = 0, k = 0;
        while (i < datlen) {
                j = 0;
                while (j < cd->decode_seq[k]) {
                        unpacked_data[i] = cd->data[k];
                        ++i; ++j;
                }
                if (j == 0)
                        ++i; // Increment i if the inner loop didn't execute

                ++k; // Move on the next element in cd->data & cd->decode_seq
        }

        return unpacked_data;        
}

int main(int argc, char* argv[])
{
        size_t unpack = 0;
        
        if (argc == 1 || !strcmp(argv[1], "-help"))
                goto help;

        if (argc > MAX_INDEX_NO_UNPACK) {
                if (strcmp(argv[UNPACK_INDEX_IF_USED], "-unpack")) {
                        printf ("scomp: Illegal arguments or unbounded string. Abort.\n");
                        return 1;
                }
                else {
                        unpack = 1;
                }
        }
        
        if (!unpack) {
                char* data = argv[NO_UNPACK_STR_INDEX];
                        
                struct compressed_data* cd = compress(data);

                if (!cd) {
                        goto memerr;
                }
                else {
                        if (!cd->decode_seq || !cd->data) {
                                free(cd->decode_seq);
                                free(cd->data);
                                free(cd);
                                goto memerr;
                        }
                        else {
                                size_t len = strlen(cd->data);
                                printf ("Compressed string: \"%s\"\n", cd->data);
                                printf ("Decode sequence: {");
                                for (size_t i = 0; i < len-1; ++i)
                                        printf ("%zu, ", cd->decode_seq[i]);
                                printf ("%zu}\n", cd->decode_seq[len-1]);
                                
                                free(cd->decode_seq);
                                free(cd->data);
                                free(cd);
                        }
                }
        }

        else {
                
                if (argc < MIN_ARGS_FOR_UNPACK) {
                        printf ("scomp: Too few arguments for unpacking. Abort.\n");
                        return 1;
                }
                
                if (strcmp(argv[DSEQ_ARG_INDEX], "-dseq")) {
                        printf ("scomp: error: -dseq unspecified or unbounded string. Abort.\n");
                        return 1;
                }

                struct compressed_data* cd = malloc(sizeof(*cd));
                if (!cd)
                        goto memerr;
                
                size_t no_of_unique_char  = argc-FIRST_DSEQ_ELEM_INDEX;
                cd->data = argv[UNPACK_STRING_INDEX];
                 
                cd->decode_seq = malloc(no_of_unique_char*sizeof(size_t));
                
                if (!cd->decode_seq) {
                        free(cd);
                        goto memerr;
                }
                
                for (size_t i = 0; i < no_of_unique_char &&
                i+FIRST_DSEQ_ELEM_INDEX < (size_t)argc; ++i)
                        cd->decode_seq[i] = atoi(argv[i+FIRST_DSEQ_ELEM_INDEX]);

                printf ("Compressed string: \"%s\"\n", cd->data);
                printf ("Decode sequence: {");
                
                for (size_t i = 0; i < no_of_unique_char-1; ++i)
                        printf ("%zu, ", cd->decode_seq[i]);

                printf ("%zu}\n", cd->decode_seq[no_of_unique_char-1]);
                
                char* unpacked_data = decompress(cd);

                if (!unpacked_data)
                        goto memerr;
           
                printf ("Decompressed string: \"%s\"\n", unpacked_data);
                
                free(cd->decode_seq);
                free(cd);
        }
        
        return 0;
                        
        memerr:
               printf ("scomp: error: buffer allocation failed due to low memory.\n");
               return 1;
        
        help:
             printf ("Usage: scomp  [-unpack] \"<string>\" [-dseq <1st> ... <nth>]\n\n");
             printf ("Compress a given string and display it with the decode sequence.\n");
             printf ("If -unpack is specified -dseq must also be, followed by the decode");
             printf (" sequence.\n\nCopyright (c) Arijit Kumar Das under MIT License.\n\n");
             return 0;
}