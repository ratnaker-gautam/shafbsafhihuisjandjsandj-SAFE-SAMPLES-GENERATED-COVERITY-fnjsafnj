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
    
    printf("Enter text to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    if (len >= MAX_INPUT - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    i = 0;
    while (i < len) {
        if (isalpha((unsigned char)input[i])) {
            int count = 1;
            char current = input[i];
            
            k = i + 1;
            while (k < len && input[k] == current) {
                count++;
                k++;
            }
            
            if (j + 4 >= sizeof(processed)) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            j += snprintf(processed + j, sizeof(processed) - j, "%c%d", current, count);
            i = k;
        } else {
            if (j + 1 >= sizeof(processed)) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            processed[j++] = input[i++];
        }
    }
    processed[j] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    char reversed[MAX_INPUT];
    int rev_idx = 0;
    for (int idx = len - 1; idx >= 0; idx--) {
        if (rev_idx >= sizeof(reversed) - 1) {
            fprintf(stderr, "Reversal buffer overflow\n");
            return 1;
        }
        reversed[rev_idx++] = input[idx];
    }
    reversed[rev_idx] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    int word_count = 0;
    int in_word = 0;
    for (int idx = 0; idx < len; idx++) {
        if (isalpha((unsigned char)input[idx])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    
    printf("Word count: %d\n", word_count);
    
    return 0;
}