//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char output[MAX_LEN];
    int i, j, k;
    int word_count = 0;
    int in_word = 0;
    
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
        printf("Empty string entered\n");
        return 0;
    }
    
    if (len >= MAX_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (i = 0; i < len; i++) {
        if (!isprint((unsigned char)input[i])) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    i = 0;
    j = 0;
    while (i < len) {
        if (isspace((unsigned char)input[i])) {
            in_word = 0;
            i++;
            continue;
        }
        
        if (!in_word) {
            word_count++;
            in_word = 1;
        }
        
        int word_start = i;
        while (i < len && !isspace((unsigned char)input[i])) {
            i++;
        }
        int word_end = i - 1;
        
        for (k = word_start; k <= word_end; k++) {
            if (j >= MAX_LEN - 1) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            output[j++] = input[k];
        }
        
        if (i < len) {
            if (j >= MAX_LEN - 1) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            output[j++] = ' ';
        }
    }
    
    if (j > 0 && output[j - 1] == ' ') {
        j--;
    }
    output[j] = '\0';
    
    printf("Original string: \"%s\"\n", input);
    printf("Processed string: \"%s\"\n", output);
    printf("Word count: %d\n", word_count);
    
    char reversed[MAX_LEN];
    int rev_idx = 0;
    i = len - 1;
    while (i >= 0) {
        if (rev_idx >= MAX_LEN - 1) {
            fprintf(stderr, "Reversed buffer overflow\n");
            return 1;
        }
        reversed[rev_idx++] = input[i];
        i--;
    }
    reversed[rev_idx] = '\0';
    printf("Reversed string: \"%s\"\n", reversed);
    
    return 0;
}