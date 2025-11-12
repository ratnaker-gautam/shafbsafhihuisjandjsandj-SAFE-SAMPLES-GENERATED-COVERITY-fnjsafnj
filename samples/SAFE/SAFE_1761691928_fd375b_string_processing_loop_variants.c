//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256
#define MAX_WORDS 50

int main() {
    char input[MAX_INPUT + 1];
    char words[MAX_WORDS][MAX_INPUT + 1];
    int word_count = 0;
    
    printf("Enter a sentence: ");
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
        printf("Empty input\n");
        return 0;
    }
    
    if (len > MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        int start = i;
        while (i < len && !isspace(input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_len <= MAX_INPUT) {
            strncpy(words[word_count], input + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        printf("Word %d: ", j + 1);
        
        int k = 0;
        do {
            if (k < strlen(words[j])) {
                putchar(words[j][k]);
                k++;
            }
        } while (k < strlen(words[j]));
        putchar('\n');
    }
    
    printf("Processing words:\n");
    int j = 0;
    while (j < word_count) {
        int has_digit = 0;
        for (int k = 0; k < strlen(words[j]); k++) {
            if (isdigit(words[j][k])) {
                has_digit = 1;
                break;
            }
        }
        
        if (has_digit) {
            printf("'%s' contains digits\n", words[j]);
        } else {
            printf("'%s' contains no digits\n", words[j]);
        }
        j++;
    }
    
    printf("Word lengths:\n");
    int total_length = 0;
    for (int j = 0; j < word_count; j++) {
        int length = 0;
        char *ptr = words[j];
        while (*ptr != '\0') {
            length++;
            ptr++;
        }
        total_length += length;
        printf("'%s' length: %d\n", words[j], length);
    }
    
    if (word_count > 0) {
        printf("Average word length: %.2f\n", (float)total_length / word_count);
    }
    
    return 0;
}