//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024

int main() {
    char input[MAX_INPUT];
    char processed[MAX_INPUT * 2];
    int i, j, k;
    int word_count = 0;
    int in_word = 0;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (len >= MAX_INPUT - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    i = 0;
    while (i < len) {
        if (isalpha((unsigned char)input[i])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
            processed[j++] = toupper((unsigned char)input[i]);
        } else if (isspace((unsigned char)input[i])) {
            if (in_word) {
                processed[j++] = ' ';
                in_word = 0;
            }
        } else {
            processed[j++] = input[i];
        }
        i++;
    }
    
    if (j > 0 && processed[j - 1] == ' ') {
        j--;
    }
    processed[j] = '\0';
    
    if (j >= sizeof(processed) - 1) {
        fprintf(stderr, "Processing overflow\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Word count: %d\n", word_count);
    
    char reversed[MAX_INPUT * 2];
    int rev_len = j;
    for (k = 0; k < rev_len; k++) {
        reversed[k] = processed[rev_len - 1 - k];
    }
    reversed[rev_len] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    int char_count[256] = {0};
    for (k = 0; k < rev_len; k++) {
        unsigned char c = (unsigned char)reversed[k];
        if (c < 256) {
            char_count[c]++;
        }
    }
    
    printf("Character frequencies:\n");
    for (k = 0; k < 256; k++) {
        if (char_count[k] > 0 && isprint(k)) {
            printf("'%c': %d\n", k, char_count[k]);
        }
    }
    
    return 0;
}