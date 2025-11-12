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
    
    for (size_t i = 0; i < len; i++) {
        if (proc_idx >= sizeof(processed) - 10) {
            fprintf(stderr, "Output buffer overflow\n");
            return 1;
        }
        
        if (isalpha(input[i])) {
            char ch = input[i];
            if (isupper(ch)) {
                ch = tolower(ch);
            } else {
                ch = toupper(ch);
            }
            processed[proc_idx++] = ch;
            processed[proc_idx++] = ch;
        } else if (isdigit(input[i])) {
            int digit = input[i] - '0';
            if (digit >= 0 && digit <= 9) {
                for (int j = 0; j < digit; j++) {
                    if (proc_idx < sizeof(processed) - 1) {
                        processed[proc_idx++] = '*';
                    }
                }
            }
        } else {
            if (proc_idx < sizeof(processed) - 1) {
                processed[proc_idx++] = input[i];
            }
        }
    }
    
    processed[proc_idx] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    size_t temp_len = 0;
    char *ptr = processed;
    while (*ptr != '\0' && temp_len < sizeof(temp) - 1) {
        if (isalnum(*ptr)) {
            temp[temp_len++] = *ptr;
        }
        ptr++;
    }
    temp[temp_len] = '\0';
    
    printf("Alphanumeric only: %s\n", temp);
    
    size_t word_count = 0;
    int in_word = 0;
    for (size_t i = 0; i < strlen(processed); i++) {
        if (isalpha(processed[i])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    
    printf("Word count: %zu\n", word_count);
    
    char reversed[MAX_LEN];
    size_t rev_idx = 0;
    size_t proc_len = strlen(processed);
    if (proc_len > 0) {
        for (size_t i = proc_len; i > 0; i--) {
            if (rev_idx < sizeof(reversed) - 1) {
                reversed[rev_idx++] = processed[i - 1];
            }
        }
    }
    reversed[rev_idx] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    return 0;
}