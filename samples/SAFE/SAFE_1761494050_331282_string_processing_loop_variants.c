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
    int len;
    
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
            output[j++] = input[i];
        }
    }
    output[j] = '\0';
    
    printf("Alphanumeric characters: %s\n", output);
    
    int word_count = 0;
    int in_word = 0;
    k = 0;
    while (k < len) {
        if (isspace((unsigned char)input[k])) {
            in_word = 0;
        } else if (!in_word) {
            word_count++;
            in_word = 1;
        }
        k++;
    }
    printf("Word count: %d\n", word_count);
    
    char reversed[MAX_LEN];
    int m = len - 1;
    int n = 0;
    do {
        if (m >= 0) {
            reversed[n++] = input[m--];
        }
    } while (m >= 0);
    reversed[n] = '\0';
    printf("Reversed string: %s\n", reversed);
    
    char uppercase[MAX_LEN];
    int p = 0;
    int q = 0;
    while (input[p] != '\0') {
        uppercase[q++] = toupper((unsigned char)input[p++]);
    }
    uppercase[q] = '\0';
    printf("Uppercase: %s\n", uppercase);
    
    int digit_count = 0;
    for (int r = 0; r < len; r++) {
        if (isdigit((unsigned char)input[r])) {
            digit_count++;
        }
    }
    printf("Digit count: %d\n", digit_count);
    
    return 0;
}