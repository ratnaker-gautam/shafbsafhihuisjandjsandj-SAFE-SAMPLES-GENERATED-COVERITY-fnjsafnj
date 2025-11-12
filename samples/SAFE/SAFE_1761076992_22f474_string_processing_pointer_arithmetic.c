//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

int main() {
    char input[MAX_INPUT_LEN];
    char *words[MAX_WORDS];
    int word_count = 0;
    
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *start = input;
    char *end = input;
    
    while (*start && word_count < MAX_WORDS) {
        while (*start && isspace((unsigned char)*start)) {
            start++;
        }
        
        if (!*start) {
            break;
        }
        
        end = start;
        while (*end && !isspace((unsigned char)*end)) {
            end++;
        }
        
        size_t word_len = end - start;
        if (word_len == 0) {
            break;
        }
        
        if (word_count >= MAX_WORDS) {
            fprintf(stderr, "Too many words\n");
            break;
        }
        
        char *word = malloc(word_len + 1);
        if (word == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            break;
        }
        
        char *dest = word;
        char *src = start;
        while (src < end) {
            *dest = *src;
            dest++;
            src++;
        }
        *dest = '\0';
        
        words[word_count] = word;
        word_count++;
        
        start = end;
    }
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char *temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
    
    for (int i = 0; i < word_count; i++) {
        if (printf("%s\n", words[i]) < 0) {
            fprintf(stderr, "Output error\n");
            break;
        }
        free(words[i]);
    }
    
    return 0;
}