//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int main() {
    char input[MAX_INPUT_LEN];
    char *p, *q;
    size_t len;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input);
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
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
        fprintf(stderr, "Input contains only whitespace\n");
        return 1;
    }
    
    q = input + len - 1;
    while (q > p && isspace((unsigned char)*q)) {
        *q = '\0';
        q--;
    }
    
    printf("Trimmed: [%s]\n", p);
    
    char *start = p;
    char *end = p;
    int word_count = 0;
    
    while (*end) {
        while (*end && !isspace((unsigned char)*end)) {
            end++;
        }
        
        if (end > start) {
            word_count++;
            
            char *rev = end - 1;
            while (rev >= start) {
                putchar(*rev);
                rev--;
            }
            putchar(' ');
        }
        
        while (*end && isspace((unsigned char)*end)) {
            end++;
        }
        start = end;
    }
    
    printf("\nWord count: %d\n", word_count);
    
    p = input;
    int has_alpha = 0;
    int has_digit = 0;
    
    while (*p) {
        if (isalpha((unsigned char)*p)) {
            has_alpha = 1;
        } else if (isdigit((unsigned char)*p)) {
            has_digit = 1;
        }
        p++;
    }
    
    if (has_alpha && has_digit) {
        printf("Contains both letters and digits\n");
    } else if (has_alpha) {
        printf("Contains only letters\n");
    } else if (has_digit) {
        printf("Contains only digits\n");
    } else {
        printf("Contains neither letters nor digits\n");
    }
    
    p = input;
    char *max_word_start = p;
    char *max_word_end = p;
    size_t max_len = 0;
    
    while (*p) {
        char *word_start = p;
        while (*p && !isspace((unsigned char)*p)) {
            p++;
        }
        
        size_t word_len = p - word_start;
        if (word_len > max_len) {
            max_len = word_len;
            max_word_start = word_start;
            max_word_end = p;
        }
        
        while (*p && isspace((unsigned char)*p)) {
            p++;
        }
    }
    
    if (max_len > 0) {
        printf("Longest word (%zu chars): ", max_len);
        char *ptr = max_word_start;
        while (ptr < max_word_end) {
            putchar(*ptr);
            ptr++;
        }
        putchar('\n');
    }
    
    return 0;
}