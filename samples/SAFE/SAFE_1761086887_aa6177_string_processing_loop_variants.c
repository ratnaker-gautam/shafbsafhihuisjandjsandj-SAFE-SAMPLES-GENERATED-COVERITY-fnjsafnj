//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
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
    while (k < j) {
        if (isalnum((unsigned char)output[k])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        k++;
    }
    printf("Word count: %d\n", word_count);
    
    char reversed[MAX_LEN];
    int m, n;
    m = 0;
    n = j - 1;
    do {
        if (n >= 0) {
            reversed[m] = output[n];
            m++;
            n--;
        }
    } while (n >= 0);
    reversed[m] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    int char_count[256] = {0};
    int p;
    for (p = 0; p < j; p++) {
        unsigned char c = (unsigned char)output[p];
        if (c < 256) {
            char_count[c]++;
        }
    }
    
    printf("Character frequencies:\n");
    int q;
    for (q = 0; q < 256; q++) {
        if (char_count[q] > 0 && isprint(q)) {
            printf("'%c': %d\n", q, char_count[q]);
        }
    }
    
    return 0;
}