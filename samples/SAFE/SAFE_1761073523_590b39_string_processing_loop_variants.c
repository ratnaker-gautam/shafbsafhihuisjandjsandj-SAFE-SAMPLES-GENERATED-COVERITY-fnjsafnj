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
    
    for (size_t i = 0; i < len; i++) {
        if (proc_idx >= sizeof(processed) - 10) {
            fprintf(stderr, "Output buffer overflow\n");
            return 1;
        }
        
        if (isalpha((unsigned char)input[i])) {
            char current = input[i];
            int count = 1;
            
            size_t j = i + 1;
            while (j < len && input[j] == current) {
                count++;
                j++;
            }
            
            if (count > 1) {
                int written = snprintf(processed + proc_idx, 
                                     sizeof(processed) - proc_idx,
                                     "%c%d", current, count);
                if (written < 0 || (size_t)written >= sizeof(processed) - proc_idx) {
                    fprintf(stderr, "Formatting error\n");
                    return 1;
                }
                proc_idx += written;
            } else {
                processed[proc_idx++] = current;
                processed[proc_idx] = '\0';
            }
            
            i = j - 1;
        } else if (isdigit((unsigned char)input[i])) {
            size_t start = i;
            while (i < len && isdigit((unsigned char)input[i])) {
                i++;
            }
            
            size_t num_len = i - start;
            if (num_len > sizeof(temp) - 1) {
                fprintf(stderr, "Number too long\n");
                return 1;
            }
            
            strncpy(temp, input + start, num_len);
            temp[num_len] = '\0';
            
            long number = strtol(temp, NULL, 10);
            if (number == 0 && temp[0] != '0') {
                fprintf(stderr, "Number conversion failed\n");
                return 1;
            }
            
            int written = snprintf(processed + proc_idx, 
                                 sizeof(processed) - proc_idx,
                                 "[%ld]", number * 2);
            if (written < 0 || (size_t)written >= sizeof(processed) - proc_idx) {
                fprintf(stderr, "Formatting error\n");
                return 1;
            }
            proc_idx += written;
            
            i--;
        } else {
            processed[proc_idx++] = input[i];
            processed[proc_idx] = '\0';
        }
    }
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    size_t k = 0;
    int word_count = 0;
    int in_word = 0;
    
    while (k < proc_idx) {
        if (isalpha((unsigned char)processed[k])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        k++;
    }
    
    printf("Word count in processed string: %d\n", word_count);
    
    return 0;
}