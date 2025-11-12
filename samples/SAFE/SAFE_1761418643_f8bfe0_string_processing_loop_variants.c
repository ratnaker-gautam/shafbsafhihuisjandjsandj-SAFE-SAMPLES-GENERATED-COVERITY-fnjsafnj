//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024

int main() {
    char input[MAX_INPUT];
    char processed[MAX_INPUT];
    
    printf("Enter a string to process: ");
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
        fprintf(stderr, "Empty input string\n");
        return 1;
    }
    
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalnum((unsigned char)input[i])) {
            if (j < sizeof(processed) - 1) {
                processed[j++] = input[i];
            } else {
                break;
            }
        }
    }
    processed[j] = '\0';
    
    size_t processed_len = strlen(processed);
    if (processed_len == 0) {
        fprintf(stderr, "No alphanumeric characters found\n");
        return 1;
    }
    
    char reversed[MAX_INPUT];
    size_t k = 0;
    size_t m = processed_len;
    while (m > 0) {
        m--;
        reversed[k++] = processed[m];
    }
    reversed[k] = '\0';
    
    char vowels_removed[MAX_INPUT];
    size_t v = 0;
    size_t n = 0;
    do {
        char c = processed[n];
        if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u' &&
            c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U') {
            if (v < sizeof(vowels_removed) - 1) {
                vowels_removed[v++] = c;
            } else {
                break;
            }
        }
        n++;
    } while (n < processed_len);
    vowels_removed[v] = '\0';
    
    char case_swapped[MAX_INPUT];
    size_t p = 0;
    for (size_t q = 0; q < processed_len; q++) {
        char ch = processed[q];
        if (islower((unsigned char)ch)) {
            case_swapped[p] = toupper((unsigned char)ch);
        } else if (isupper((unsigned char)ch)) {
            case_swapped[p] = tolower((unsigned char)ch);
        } else {
            case_swapped[p] = ch;
        }
        p++;
    }
    case_swapped[p] = '\0';
    
    printf("Original processed: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Vowels removed: %s\n", vowels_removed);
    printf("Case swapped: %s\n", case_swapped);
    
    size_t char_count[256] = {0};
    for (size_t r = 0; r < processed_len; r++) {
        unsigned char idx = (unsigned char)processed[r];
        if (idx < 256) {
            char_count[idx]++;
        }
    }
    
    printf("Character frequencies:\n");
    for (int s = 0; s < 256; s++) {
        if (char_count[s] > 0 && isprint(s)) {
            printf("'%c': %zu\n", s, char_count[s]);
        }
    }
    
    return 0;
}