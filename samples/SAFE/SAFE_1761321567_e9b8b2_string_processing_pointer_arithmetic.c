//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char output[MAX_LEN];
    char *src, *dst;
    int word_count = 0;
    int in_word = 0;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    src = input;
    dst = output;
    char *end = input + len;
    
    while (src < end) {
        if (isalpha((unsigned char)*src)) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
            *dst++ = *src;
        } else if (isspace((unsigned char)*src)) {
            if (in_word) {
                in_word = 0;
                *dst++ = ' ';
            }
        } else {
            *dst++ = ' ';
        }
        src++;
    }
    
    if (dst > output && *(dst - 1) == ' ') {
        dst--;
    }
    *dst = '\0';
    
    if (dst - output >= sizeof(output)) {
        fprintf(stderr, "Output buffer overflow\n");
        return 1;
    }
    
    printf("Processed string: \"%s\"\n", output);
    printf("Word count: %d\n", word_count);
    
    char reversed[MAX_LEN];
    char *rev_ptr = reversed;
    char *word_start = output;
    char *word_end = output;
    
    while (*word_end) {
        if (*word_end == ' ') {
            if (word_start < word_end) {
                char *p = word_end - 1;
                while (p >= word_start) {
                    *rev_ptr++ = *p;
                    p--;
                }
                *rev_ptr++ = ' ';
            }
            word_start = word_end + 1;
        }
        word_end++;
    }
    
    if (word_start < word_end) {
        char *p = word_end - 1;
        while (p >= word_start) {
            *rev_ptr++ = *p;
            p--;
        }
    }
    
    if (rev_ptr > reversed && *(rev_ptr - 1) == ' ') {
        rev_ptr--;
    }
    *rev_ptr = '\0';
    
    if (rev_ptr - reversed >= sizeof(reversed)) {
        fprintf(stderr, "Reversed buffer overflow\n");
        return 1;
    }
    
    printf("Words reversed: \"%s\"\n", reversed);
    
    return 0;
}