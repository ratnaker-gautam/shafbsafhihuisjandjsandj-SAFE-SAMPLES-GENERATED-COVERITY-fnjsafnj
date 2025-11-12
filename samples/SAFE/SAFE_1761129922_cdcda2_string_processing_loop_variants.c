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
    
    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
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
    for (i = 0; i < len; i++) {
        if (isalpha((unsigned char)input[i])) {
            output[j++] = toupper((unsigned char)input[i]);
        }
    }
    output[j] = '\0';
    
    printf("Uppercase letters only: %s\n", output);
    
    int letter_count = 0;
    i = 0;
    while (output[i] != '\0') {
        letter_count++;
        i++;
    }
    
    printf("Letter count: %d\n", letter_count);
    
    char reversed[MAX_LEN];
    k = 0;
    for (i = letter_count - 1; i >= 0; i--) {
        reversed[k++] = output[i];
    }
    reversed[k] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    int vowel_count = 0;
    i = 0;
    do {
        char c = reversed[i];
        if (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
            vowel_count++;
        }
        i++;
    } while (reversed[i] != '\0');
    
    printf("Vowel count in reversed: %d\n", vowel_count);
    
    char processed[MAX_LEN];
    int processed_len = 0;
    
    for (i = 0; i < letter_count; i++) {
        if (i % 2 == 0) {
            processed[processed_len++] = output[i];
        }
    }
    processed[processed_len] = '\0';
    
    printf("Every second character: %s\n", processed);
    
    int sum_ascii = 0;
    for (i = 0; i < processed_len; i++) {
        sum_ascii += (int)processed[i];
    }
    
    printf("Sum of ASCII values: %d\n", sum_ascii);
    
    return 0;
}