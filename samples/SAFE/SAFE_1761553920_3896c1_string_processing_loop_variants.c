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
    for (k = 0; k < len; k++) {
        if (isspace((unsigned char)input[k])) {
            in_word = 0;
        } else {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        }
    }
    printf("Word count: %d\n", word_count);
    
    char reversed[MAX_LEN];
    int m, n;
    for (m = len - 1, n = 0; m >= 0; m--, n++) {
        if (n < MAX_LEN - 1) {
            reversed[n] = input[m];
        } else {
            break;
        }
    }
    reversed[n] = '\0';
    printf("Reversed string: %s\n", reversed);
    
    int digit_sum = 0;
    int p = 0;
    while (p < len) {
        if (isdigit((unsigned char)input[p])) {
            digit_sum += (input[p] - '0');
        }
        p++;
    }
    printf("Sum of digits: %d\n", digit_sum);
    
    char uppercase[MAX_LEN];
    int q;
    for (q = 0; q < len; q++) {
        if (q < MAX_LEN - 1) {
            uppercase[q] = toupper((unsigned char)input[q]);
        } else {
            break;
        }
    }
    uppercase[q < MAX_LEN ? q : MAX_LEN - 1] = '\0';
    printf("Uppercase: %s\n", uppercase);
    
    return 0;
}