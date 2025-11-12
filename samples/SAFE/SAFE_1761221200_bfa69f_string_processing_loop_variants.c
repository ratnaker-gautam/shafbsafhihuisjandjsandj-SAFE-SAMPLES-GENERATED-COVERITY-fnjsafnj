//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char processed[MAX_LEN];
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
        printf("Empty string provided\n");
        return 0;
    }
    
    if (len >= MAX_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    for (i = 0; i < len; i++) {
        if (isalnum((unsigned char)input[i])) {
            processed[j] = input[i];
            j++;
        }
    }
    processed[j] = '\0';
    
    int processed_len = j;
    if (processed_len == 0) {
        printf("No alphanumeric characters found\n");
        return 0;
    }
    
    char reversed[MAX_LEN];
    int k = processed_len - 1;
    while (k >= 0) {
        reversed[processed_len - 1 - k] = processed[k];
        k--;
    }
    reversed[processed_len] = '\0';
    
    char uppercase[MAX_LEN];
    int m = 0;
    do {
        if (m < processed_len) {
            uppercase[m] = toupper((unsigned char)processed[m]);
            m++;
        }
    } while (m < processed_len);
    uppercase[processed_len] = '\0';
    
    printf("Original: %s\n", input);
    printf("Alphanumeric only: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Uppercase: %s\n", uppercase);
    
    int char_count[256] = {0};
    int n = 0;
    while (n < processed_len) {
        unsigned char c = (unsigned char)processed[n];
        if (c < 256) {
            char_count[c]++;
        }
        n++;
    }
    
    printf("Character frequencies:\n");
    for (int idx = 0; idx < 256; idx++) {
        if (char_count[idx] > 0 && isalnum(idx)) {
            printf("'%c': %d\n", idx, char_count[idx]);
        }
    }
    
    return 0;
}