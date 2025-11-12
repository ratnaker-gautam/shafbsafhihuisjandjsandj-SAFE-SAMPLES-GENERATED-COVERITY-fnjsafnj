//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char output[MAX_LEN];
    int i, j, len;
    
    printf("Enter a string (max %d chars): ", MAX_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty string\n");
        return 1;
    }
    
    if (len >= MAX_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    for (i = 0; i < len; i++) {
        if (isalnum((unsigned char)input[i])) {
            output[j] = input[i];
            j++;
        }
    }
    output[j] = '\0';
    
    if (j == 0) {
        fprintf(stderr, "No alphanumeric characters found\n");
        return 1;
    }
    
    int output_len = j;
    char reversed[MAX_LEN];
    
    i = output_len - 1;
    j = 0;
    while (i >= 0) {
        reversed[j] = output[i];
        j++;
        i--;
    }
    reversed[j] = '\0';
    
    char processed[MAX_LEN];
    int k = 0;
    
    for (i = 0; i < output_len; i++) {
        if (i % 2 == 0) {
            processed[k] = toupper((unsigned char)output[i]);
        } else {
            processed[k] = tolower((unsigned char)output[i]);
        }
        k++;
    }
    processed[k] = '\0';
    
    printf("Original: %s\n", input);
    printf("Alphanumeric only: %s\n", output);
    printf("Reversed: %s\n", reversed);
    printf("Alternating case: %s\n", processed);
    
    int char_count[256] = {0};
    int max_count = 0;
    char most_common = '\0';
    
    for (i = 0; i < output_len; i++) {
        char c = output[i];
        if (isalnum((unsigned char)c)) {
            char_count[(unsigned char)c]++;
            if (char_count[(unsigned char)c] > max_count) {
                max_count = char_count[(unsigned char)c];
                most_common = c;
            }
        }
    }
    
    if (max_count > 0) {
        printf("Most common character: '%c' (appears %d times)\n", most_common, max_count);
    }
    
    return 0;
}