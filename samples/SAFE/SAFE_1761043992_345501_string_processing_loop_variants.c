//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char processed[MAX_LEN * 2];
    char temp[MAX_LEN];
    
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
    
    if (len >= MAX_LEN - 1) {
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
            if (word_len >= sizeof(temp)) {
                fprintf(stderr, "Word too long\n");
                return 1;
            }
            
            strncpy(temp, &input[word_start], word_len);
            temp[word_len] = '\0';
            
            for (size_t j = 0; j < word_len; j++) {
                if (proc_idx >= sizeof(processed) - 2) {
                    fprintf(stderr, "Output buffer overflow\n");
                    return 1;
                }
                processed[proc_idx++] = temp[j];
            }
            
            if (proc_idx >= sizeof(processed) - 2) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            processed[proc_idx++] = ' ';
        }
        
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
    }
    
    if (proc_idx > 0 && processed[proc_idx - 1] == ' ') {
        proc_idx--;
    }
    processed[proc_idx] = '\0';
    
    char reversed[MAX_LEN * 2];
    size_t rev_idx = 0;
    size_t pos = proc_idx;
    
    do {
        if (pos == 0) break;
        pos--;
        
        size_t word_end = pos;
        while (pos > 0 && !isspace((unsigned char)processed[pos - 1])) {
            pos--;
        }
        
        size_t word_len = word_end - pos + 1;
        if (rev_idx + word_len + 1 >= sizeof(reversed)) {
            fprintf(stderr, "Reversed buffer overflow\n");
            return 1;
        }
        
        for (size_t k = 0; k < word_len; k++) {
            reversed[rev_idx++] = processed[pos + k];
        }
        
        if (pos > 0) {
            reversed[rev_idx++] = ' ';
        }
    } while (pos > 0);
    
    reversed[rev_idx] = '\0';
    
    char final[MAX_LEN * 2];
    size_t final_idx = 0;
    
    for (size_t m = 0; m < rev_idx; m++) {
        if (final_idx >= sizeof(final) - 1) {
            fprintf(stderr, "Final buffer overflow\n");
            return 1;
        }
        
        char c = reversed[m];
        if (isalpha((unsigned char)c)) {
            if (islower((unsigned char)c)) {
                final[final_idx++] = toupper((unsigned char)c);
            } else {
                final[final_idx++] = tolower((unsigned char)c);
            }
        } else {
            final[final_idx++] = c;
        }
    }
    final[final_idx] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Reversed words: %s\n", reversed);
    printf("Case swapped: %s\n", final);
    
    return 0;
}