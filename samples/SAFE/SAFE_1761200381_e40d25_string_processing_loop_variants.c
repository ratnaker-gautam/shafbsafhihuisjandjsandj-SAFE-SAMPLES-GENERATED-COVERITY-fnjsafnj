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
        if (isalpha((unsigned char)output[k])) {
            letter_count++;
        }
        k++;
    }
    printf("Letter count: %d\n", letter_count);
    
    char reversed[MAX_LEN];
    int m = strlen(output);
    int n = 0;
    do {
        if (m > 0) {
            reversed[n++] = output[m - 1];
            m--;
        }
    } while (m > 0);
    reversed[n] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    int vowel_count = 0;
    char *ptr = output;
    while (*ptr) {
        char c = *ptr;
        if (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
            vowel_count++;
        }
        ptr++;
    }
    printf("Vowel count: %d\n", vowel_count);
    
    char no_vowels[MAX_LEN];
    int x = 0, y = 0;
    for (x = 0; output[x] != '\0'; x++) {
        char ch = output[x];
        if (ch != 'A' && ch != 'E' && ch != 'I' && ch != 'O' && ch != 'U') {
            no_vowels[y++] = ch;
        }
    }
    no_vowels[y] = '\0';
    
    printf("Without vowels: %s\n", no_vowels);
    
    int word_count = 0;
    int in_word = 0;
    int z = 0;
    for (z = 0; input[z] != '\0'; z++) {
        if (isalpha((unsigned char)input[z])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    printf("Word count: %d\n", word_count);
    
    return 0;
}