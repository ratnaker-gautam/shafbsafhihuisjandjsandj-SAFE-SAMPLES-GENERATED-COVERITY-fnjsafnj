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
        if (isalpha((unsigned char)input[i])) {
            output[j++] = toupper((unsigned char)input[i]);
        }
    }
    output[j] = '\0';
    
    printf("Uppercase letters only: %s\n", output);
    
    int letter_count = 0;
    k = 0;
    while (output[k] != '\0') {
        letter_count++;
        k++;
    }
    
    printf("Letter count: %d\n", letter_count);
    
    char reversed[MAX_LEN];
    int m, n;
    m = letter_count - 1;
    n = 0;
    while (m >= 0) {
        reversed[n] = output[m];
        m--;
        n++;
    }
    reversed[n] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    int vowel_count = 0;
    char c;
    for (int idx = 0; idx < letter_count; idx++) {
        c = output[idx];
        switch (c) {
            case 'A': case 'E': case 'I': case 'O': case 'U':
                vowel_count++;
                break;
            default:
                break;
        }
    }
    
    printf("Vowel count: %d\n", vowel_count);
    
    char unique[MAX_LEN];
    int unique_count = 0;
    int found;
    
    for (int p = 0; p < letter_count; p++) {
        found = 0;
        for (int q = 0; q < unique_count; q++) {
            if (unique[q] == output[p]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            unique[unique_count++] = output[p];
        }
    }
    unique[unique_count] = '\0';
    
    printf("Unique letters: %s\n", unique);
    printf("Unique count: %d\n", unique_count);
    
    return 0;
}