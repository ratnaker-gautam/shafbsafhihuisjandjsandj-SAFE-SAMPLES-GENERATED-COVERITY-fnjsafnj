//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int main(void) {
    char input[MAX_INPUT_LEN];
    char *p, *q;
    size_t len;
    
    printf("Enter a string: ");
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
        fprintf(stderr, "Empty input string\n");
        return 1;
    }
    
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    p = input;
    while (*p && isspace((unsigned char)*p)) {
        p++;
    }
    
    if (*p == '\0') {
        fprintf(stderr, "String contains only whitespace\n");
        return 1;
    }
    
    q = input + len - 1;
    while (q > p && isspace((unsigned char)*q)) {
        *q = '\0';
        q--;
    }
    
    printf("Trimmed string: \"%s\"\n", p);
    
    size_t word_count = 0;
    char *start = p;
    int in_word = 0;
    
    while (*p) {
        if (!isspace((unsigned char)*p)) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        p++;
    }
    
    printf("Word count: %zu\n", word_count);
    
    p = start;
    char *words[256];
    size_t word_index = 0;
    in_word = 0;
    
    while (*p && word_index < 255) {
        if (!isspace((unsigned char)*p)) {
            if (!in_word) {
                words[word_index] = p;
                in_word = 1;
            }
        } else {
            if (in_word) {
                *p = '\0';
                in_word = 0;
                word_index++;
            }
        }
        p++;
    }
    
    if (in_word) {
        word_index++;
    }
    
    for (size_t i = 0; i < word_index; i++) {
        for (size_t j = i + 1; j < word_index; j++) {
            if (strcmp(words[i], words[j]) > 0) {
                char *temp = words[i];
                words[i] = words[j];
                words[j] = temp;
            }
        }
    }
    
    printf("Sorted words: ");
    for (size_t i = 0; i < word_index; i++) {
        printf("%s", words[i]);
        if (i < word_index - 1) {
            printf(" ");
        }
    }
    printf("\n");
    
    p = start;
    while (*p) {
        if (islower((unsigned char)*p)) {
            *p = toupper((unsigned char)*p);
        }
        p++;
    }
    
    printf("Uppercase: \"%s\"\n", start);
    
    return 0;
}