//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int main(void) {
    char input[MAX_INPUT_LEN];
    char *p = input;
    char *end = input + MAX_INPUT_LEN - 1;
    int c;
    size_t len = 0;

    printf("Enter a string (max %d chars): ", MAX_INPUT_LEN - 1);
    
    while ((c = getchar()) != EOF && c != '\n' && p < end) {
        *p++ = (char)c;
        len++;
    }
    *p = '\0';

    if (len == 0) {
        printf("No input provided.\n");
        return 1;
    }

    printf("Original: %s\n", input);
    printf("Length: %zu\n", len);

    char *start = input;
    char *q = input;
    int word_count = 0;
    int in_word = 0;

    while (*start) {
        if (isalpha((unsigned char)*start)) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
            *q++ = *start;
        } else if (isspace((unsigned char)*start)) {
            if (in_word) {
                *q++ = ' ';
                in_word = 0;
            }
        }
        start++;
    }

    if (q > input && *(q - 1) == ' ') {
        q--;
    }
    *q = '\0';

    printf("Words only: %s\n", input);
    printf("Word count: %d\n", word_count);

    char *rev_start = input;
    char *rev_end = input + strlen(input) - 1;

    while (rev_start < rev_end) {
        char temp = *rev_start;
        *rev_start = *rev_end;
        *rev_end = temp;
        rev_start++;
        rev_end--;
    }

    printf("Reversed: %s\n", input);

    char search[MAX_INPUT_LEN];
    printf("Enter substring to search: ");
    p = search;
    while ((c = getchar()) != EOF && c != '\n' && p < search + MAX_INPUT_LEN - 1) {
        *p++ = (char)c;
    }
    *p = '\0';

    if (strlen(search) > 0) {
        char *found = input;
        int found_count = 0;
        size_t search_len = strlen(search);
        
        while (*found && search_len > 0) {
            char *match = found;
            char *sub = search;
            while (*match && *sub && *match == *sub) {
                match++;
                sub++;
            }
            if (!*sub) {
                found_count++;
            }
            found++;
        }
        
        printf("Substring '%s' found %d times\n", search, found_count);
    }

    return 0;
}