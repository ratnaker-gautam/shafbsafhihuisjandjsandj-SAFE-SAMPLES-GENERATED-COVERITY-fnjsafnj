//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int main() {
    char input[MAX_INPUT_LEN];
    char *p = input;
    char *end = input + MAX_INPUT_LEN - 1;
    int ch;
    size_t len = 0;

    printf("Enter a string (max %d chars): ", MAX_INPUT_LEN - 1);
    
    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (p < end) {
            *p++ = (char)ch;
            len++;
        } else {
            break;
        }
    }
    *p = '\0';

    if (len == 0) {
        printf("Empty input string.\n");
        return 1;
    }

    char *start = input;
    char *q = input;
    
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }
    
    if (*start == '\0') {
        printf("String contains only whitespace.\n");
        return 1;
    }

    char *last_char = input + len - 1;
    while (last_char > start && isspace((unsigned char)*last_char)) {
        *last_char = '\0';
        last_char--;
    }

    p = start;
    while (*p) {
        if (!isspace((unsigned char)*p)) {
            *q++ = *p;
        }
        p++;
    }
    *q = '\0';

    if (q == start) {
        printf("No non-whitespace characters found.\n");
        return 1;
    }

    p = start;
    char *mid = start + (q - start) / 2;
    char *rev = q - 1;
    
    while (p < mid) {
        char temp = *p;
        *p = *rev;
        *rev = temp;
        p++;
        rev--;
    }

    printf("Processed string: %s\n", start);
    printf("Length: %ld\n", (long)(q - start));

    p = start;
    int vowel_count = 0;
    int consonant_count = 0;
    
    while (*p) {
        char c = *p;
        if (isalpha((unsigned char)c)) {
            c = tolower((unsigned char)c);
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                vowel_count++;
            } else {
                consonant_count++;
            }
        }
        p++;
    }

    printf("Vowels: %d\n", vowel_count);
    printf("Consonants: %d\n", consonant_count);

    return 0;
}