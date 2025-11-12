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
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
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
            processed[j++] = c;
            if (c >= 'a' && c <= 'z') {
                counts[c - 'a']++;
            }
        }
    }
    processed[j] = '\0';
    
    if (j == 0) {
        printf("No alphabetic characters found\n");
        return 1;
    }
    
    printf("Processed text (letters only, lowercase): %s\n", processed);
    printf("Character frequencies:\n");
    
    int i = 0;
    while (i < 26) {
        if (counts[i] > 0) {
            printf("%c: %d\n", 'a' + i, counts[i]);
        }
        i++;
    }
    
    char reversed[MAX_INPUT];
    size_t k = 0;
    size_t m = j;
    do {
        if (m > 0) {
            reversed[k++] = processed[m - 1];
            m--;
        }
    } while (m > 0);
    reversed[k] = '\0';
    
    printf("Reversed processed text: %s\n", reversed);
    
    int vowel_count = 0;
    int consonant_count = 0;
    for (size_t n = 0; n < j; n++) {
        switch (processed[n]) {
            case 'a': case 'e': case 'i': case 'o': case 'u':
                vowel_count++;
                break;
            default:
                if (isalpha((unsigned char)processed[n])) {
                    consonant_count++;
                }
                break;
        }
    }
    
    printf("Vowels: %d, Consonants: %d\n", vowel_count, consonant_count);
    
    return 0;
}