//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char processed[MAX_LEN * 2];
    int counts[26] = {0};
    
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
    
    size_t proc_idx = 0;
    size_t i = 0;
    while (i < len) {
        char c = input[i];
        
        if (isalpha((unsigned char)c)) {
            char upper = toupper((unsigned char)c);
            processed[proc_idx++] = upper;
            
            if (upper >= 'A' && upper <= 'Z') {
                counts[upper - 'A']++;
            }
            
            if (proc_idx < sizeof(processed) - 1) {
                processed[proc_idx++] = ' ';
            }
        }
        i++;
    }
    
    if (proc_idx > 0 && processed[proc_idx - 1] == ' ') {
        proc_idx--;
    }
    processed[proc_idx] = '\0';
    
    printf("Processed: %s\n", processed);
    printf("Letter frequencies:\n");
    
    for (int j = 0; j < 26; j++) {
        if (counts[j] > 0) {
            printf("%c: %d\n", 'A' + j, counts[j]);
        }
    }
    
    char reversed[MAX_LEN];
    size_t rev_idx = 0;
    for (size_t k = len; k > 0; k--) {
        if (rev_idx < sizeof(reversed) - 1) {
            reversed[rev_idx++] = input[k - 1];
        }
    }
    reversed[rev_idx] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    int word_count = 0;
    int in_word = 0;
    size_t m = 0;
    do {
        if (m < len && isalpha((unsigned char)input[m])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        m++;
    } while (m <= len);
    
    printf("Word count: %d\n", word_count);
    
    return 0;
}