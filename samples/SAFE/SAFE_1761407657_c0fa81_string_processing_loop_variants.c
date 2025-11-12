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
    
    int i = 0;
    while (input[i] != '\0') {
        if (isalpha((unsigned char)input[i])) {
            size_t word_start = i;
            while (isalpha((unsigned char)input[i])) {
                i++;
            }
            size_t word_len = i - word_start;
            
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
                processed[proc_idx++] = toupper((unsigned char)temp[j]);
            }
            
            if (proc_idx >= sizeof(processed) - 2) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            processed[proc_idx++] = ' ';
        } else if (isdigit((unsigned char)input[i])) {
            size_t num_start = i;
            while (isdigit((unsigned char)input[i])) {
                i++;
            }
            size_t num_len = i - num_start;
            
            if (num_len >= sizeof(temp)) {
                fprintf(stderr, "Number too long\n");
                return 1;
            }
            
            strncpy(temp, &input[num_start], num_len);
            temp[num_len] = '\0';
            
            long number = strtol(temp, NULL, 10);
            if (number > 1000000 || number < -1000000) {
                fprintf(stderr, "Number out of range\n");
                return 1;
            }
            
            int written = snprintf(&processed[proc_idx], 
                                 sizeof(processed) - proc_idx, 
                                 "[%ld]", number);
            if (written < 0 || (size_t)written >= sizeof(processed) - proc_idx) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            proc_idx += written;
            
            if (proc_idx >= sizeof(processed) - 2) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            processed[proc_idx++] = ' ';
        } else {
            if (proc_idx >= sizeof(processed) - 2) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            processed[proc_idx++] = input[i];
            i++;
        }
    }
    
    if (proc_idx > 0 && processed[proc_idx - 1] == ' ') {
        proc_idx--;
    }
    processed[proc_idx] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    size_t char_count = 0;
    size_t word_count = 0;
    size_t num_count = 0;
    
    i = 0;
    do {
        if (isalpha((unsigned char)input[i])) {
            word_count++;
            while (isalpha((unsigned char)input[i])) {
                char_count++;
                i++;
            }
        } else if (isdigit((unsigned char)input[i])) {
            num_count++;
            while (isdigit((unsigned char)input[i])) {
                char_count++;
                i++;
            }
        } else {
            if (input[i] != '\0') {
                char_count++;
            }
            i++;
        }
    } while (input[i] != '\0');
    
    printf("Statistics: %zu characters, %zu words, %zu numbers\n", 
           char_count, word_count, num_count);
    
    return 0;
}