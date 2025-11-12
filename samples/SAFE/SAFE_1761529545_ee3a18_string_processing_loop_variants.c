//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int main() {
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH * 2];
    char temp[MAX_LENGTH];
    
    printf("Enter a string to process: ");
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
        printf("Empty string provided\n");
        return 0;
    }
    
    if (len >= MAX_LENGTH - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    processed[0] = '\0';
    size_t proc_idx = 0;
    
    size_t i = 0;
    while (i < len) {
        size_t word_start = i;
        while (i < len && !isspace((unsigned char)input[i])) {
            i++;
        }
        
        size_t word_len = i - word_start;
        if (word_len > 0) {
            if (proc_idx + word_len * 2 + 1 >= sizeof(processed)) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            for (size_t j = 0; j < word_len; j++) {
                temp[j] = input[word_start + j];
            }
            temp[word_len] = '\0';
            
            size_t k = 0;
            do {
                processed[proc_idx++] = temp[k];
                k++;
            } while (k < word_len);
            
            processed[proc_idx++] = ' ';
            
            for (size_t m = 0; m < word_len; m++) {
                processed[proc_idx++] = temp[m];
            }
            processed[proc_idx++] = ' ';
        }
        
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
    }
    
    if (proc_idx > 0) {
        processed[proc_idx - 1] = '\0';
    } else {
        processed[0] = '\0';
    }
    
    printf("Processed string: %s\n", processed);
    
    size_t char_count[256] = {0};
    for (size_t idx = 0; idx < strlen(processed); idx++) {
        unsigned char c = (unsigned char)processed[idx];
        if (c < 256) {
            char_count[c]++;
        }
    }
    
    printf("Character frequencies:\n");
    for (int ch = 0; ch < 256; ch++) {
        if (char_count[ch] > 0 && isprint(ch)) {
            printf("'%c': %zu\n", ch, char_count[ch]);
        }
    }
    
    return 0;
}