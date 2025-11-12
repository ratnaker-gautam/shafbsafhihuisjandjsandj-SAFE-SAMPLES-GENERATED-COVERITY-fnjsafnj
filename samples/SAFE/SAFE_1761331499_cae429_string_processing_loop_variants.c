//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char processed[MAX_LEN * 2];
    int i, j, k;
    
    printf("Enter a string (max %d chars): ", MAX_LEN - 1);
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
    
    j = 0;
    i = 0;
    while (i < len) {
        if (isalpha((unsigned char)input[i])) {
            char current = input[i];
            int count = 1;
            
            k = i + 1;
            while (k < len && input[k] == current) {
                count++;
                k++;
            }
            
            if (count > 1) {
                int written = snprintf(processed + j, sizeof(processed) - j, "%c%d", current, count);
                if (written < 0 || written >= (int)(sizeof(processed) - j)) {
                    fprintf(stderr, "Output buffer overflow\n");
                    return 1;
                }
                j += written;
            } else {
                if (j >= (int)sizeof(processed) - 1) {
                    fprintf(stderr, "Output buffer overflow\n");
                    return 1;
                }
                processed[j++] = current;
            }
            
            i = k;
        } else {
            if (j >= (int)sizeof(processed) - 1) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            processed[j++] = input[i++];
        }
    }
    
    processed[j] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    char reversed[MAX_LEN];
    int rev_idx = 0;
    for (int idx = j - 1; idx >= 0; idx--) {
        if (rev_idx >= MAX_LEN - 1) {
            break;
        }
        reversed[rev_idx++] = processed[idx];
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