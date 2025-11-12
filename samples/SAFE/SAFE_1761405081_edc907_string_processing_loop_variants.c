//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024

int main() {
    char input[MAX_INPUT];
    char processed[MAX_INPUT];
    int counts[26] = {0};
    
    printf("Enter text to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalpha((unsigned char)input[i])) {
            char c = tolower((unsigned char)input[i]);
            counts[c - 'a']++;
        }
    }
    
    int k = 0;
    while (k < 26) {
        if (counts[k] > 0) {
            printf("Letter '%c': %d occurrences\n", 'a' + k, counts[k]);
        }
        k++;
    }
    
    int m = 0;
    int n = 0;
    do {
        if (m < len) {
            char current = input[m];
            if (isalpha((unsigned char)current)) {
                processed[n] = tolower((unsigned char)current);
                n++;
            }
            m++;
        }
    } while (m < len);
    processed[n] = '\0';
    
    printf("Alphabetic characters only: %s\n", processed);
    
    char reversed[MAX_INPUT];
    size_t r = 0;
    for (size_t p = n; p > 0; p--) {
        reversed[r] = processed[p-1];
        r++;
    }
    reversed[r] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    int vowel_count = 0;
    int consonant_count = 0;
    char *ptr = processed;
    while (*ptr) {
        char ch = *ptr;
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
            vowel_count++;
        } else {
            consonant_count++;
        }
        ptr++;
    }
    
    printf("Vowels: %d, Consonants: %d\n", vowel_count, consonant_count);
    
    return 0;
}