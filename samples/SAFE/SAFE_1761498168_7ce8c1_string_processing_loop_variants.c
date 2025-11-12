//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
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
            
            size_t j = 0;
            size_t k = 0;
            for (j = 0; j < word_len; j++) {
                if (isalpha((unsigned char)temp[j])) {
                    temp[k++] = toupper((unsigned char)temp[j]);
                }
            }
            temp[k] = '\0';
            
            if (proc_idx + k + 2 >= sizeof(processed)) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            for (j = 0; j < k; j++) {
                processed[proc_idx++] = temp[j];
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
    size_t total_chars = 0;
    
    for (i = 0; processed[i] != '\0'; i++) {
        unsigned char c = (unsigned char)processed[i];
        if (c < 256) {
            char_count[c]++;
            total_chars++;
        }
    }
    
    if (total_chars > 0) {
        printf("Character frequencies:\n");
        for (i = 0; i < 256; i++) {
            if (char_count[i] > 0 && isprint((unsigned char)i)) {
                printf("'%c': %zu\n", (char)i, char_count[i]);
            }
        }
    }
    
    return 0;
}