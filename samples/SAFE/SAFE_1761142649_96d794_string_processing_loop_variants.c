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
            
            if (j + 3 >= sizeof(processed)) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            processed[j++] = current;
            if (count > 1) {
                char num_str[12];
                int written = snprintf(num_str, sizeof(num_str), "%d", count);
                if (written < 0 || (size_t)written >= sizeof(num_str)) {
                    fprintf(stderr, "Number conversion error\n");
                    return 1;
                }
                
                for (int m = 0; m < written; m++) {
                    if (j >= sizeof(processed) - 1) {
                        fprintf(stderr, "Output buffer overflow\n");
                        return 1;
                    }
                    processed[j++] = num_str[m];
                }
            }
            
            i = k;
        } else {
            if (j >= sizeof(processed) - 1) {
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
    size_t rev_len = strlen(processed);
    if (rev_len >= sizeof(reversed)) {
        fprintf(stderr, "Reversal buffer overflow\n");
        return 1;
    }
    
    for (size_t idx = 0; idx < rev_len; idx++) {
        reversed[idx] = processed[rev_len - 1 - idx];
    }
    reversed[rev_len] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    int vowel_count = 0;
    for (size_t idx = 0; idx < rev_len; idx++) {
        char c = tolower((unsigned char)reversed[idx]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            vowel_count++;
        }
    }
    
    printf("Vowels in reversed: %d\n", vowel_count);
    
    return 0;
}