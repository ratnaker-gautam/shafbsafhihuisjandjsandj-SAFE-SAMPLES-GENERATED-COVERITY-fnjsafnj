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
            
            for (size_t j = 0; j < word_len; j++) {
                if (proc_idx >= sizeof(processed) - 2) {
                    fprintf(stderr, "Output buffer overflow\n");
                    return 1;
                }
                processed[proc_idx++] = temp[j];
            }
            
            if (proc_idx >= sizeof(processed) - 3) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            processed[proc_idx++] = ' ';
            processed[proc_idx++] = '[';
            
            int digit_count = 0;
            for (size_t j = 0; j < word_len; j++) {
                if (isdigit((unsigned char)temp[j])) {
                    digit_count++;
                }
            }
            
            char count_str[16];
            int written = snprintf(count_str, sizeof(count_str), "%d", digit_count);
            if (written < 0 || (size_t)written >= sizeof(count_str)) {
                fprintf(stderr, "Number formatting error\n");
                return 1;
            }
            
            for (int j = 0; count_str[j] != '\0'; j++) {
                if (proc_idx >= sizeof(processed) - 1) {
                    fprintf(stderr, "Output buffer overflow\n");
                    return 1;
                }
                processed[proc_idx++] = count_str[j];
            }
            
            if (proc_idx >= sizeof(processed) - 1) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            processed[proc_idx++] = ']';
            processed[proc_idx++] = ' ';
        }
        
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
    }
    
    if (proc_idx > 0 && processed[proc_idx - 1] == ' ') {
        proc_idx--;
    }
    
    if (proc_idx >= sizeof(processed)) {
        fprintf(stderr, "Output buffer overflow\n");
        return 1;
    }
    processed[proc_idx] = '\0';
    
    printf("Processed: %s\n", processed);
    
    char reversed[MAX_LEN];
    size_t rev_idx = 0;
    
    for (size_t pos = len; pos > 0; pos--) {
        if (rev_idx >= sizeof(reversed) - 1) {
            fprintf(stderr, "Reversal buffer overflow\n");
            return 1;
        }
        reversed[rev_idx++] = input[pos - 1];
    }
    reversed[rev_idx] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    return 0;
}