//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')

typedef struct {
    char text[MAX_WORD_LEN];
    int length;
} Word;

int safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int parse_sentence(const char *input, Word words[], int max_words) {
    int word_count = 0;
    int pos = 0;
    int input_len = safe_strlen(input, MAX_INPUT_LEN);
    
    while (pos < input_len && word_count < max_words) {
        while (pos < input_len && IS_WHITESPACE(input[pos])) {
            pos++;
        }
        
        if (pos >= input_len) {
            break;
        }
        
        int word_start = pos;
        int word_len = 0;
        
        while (pos < input_len && !IS_WHITESPACE(input[pos]) && !IS_TERMINATOR(input[pos])) {
            if (word_len < MAX_WORD_LEN - 1) {
                words[word_count].text[word_len] = input[pos];
                word_len++;
            }
            pos++;
        }
        
        if (word_len > 0) {
            words[word_count].text[word_len] = '\0';
            words[word_count].length = word_len;
            word_count++;
        }
        
        if (pos < input_len && IS_TERMINATOR(input[pos])) {
            break;
        }
    }
    
    return word_count;
}

void capitalize_words(Word words[], int count) {
    for (int i = 0; i < count; i++) {
        if (words[i].length > 0 && islower(words[i].text[0])) {
            words[i].text[0] = toupper(words[i].text[0]);
        }
    }
}

void reverse_words(Word words[], int count) {
    int i = 0;
    int j = count - 1;
    
    while (i < j) {
        Word temp = words[i];
        words[i] = words[j];
        words[j] = temp;
        i++;
        j--;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    Word words[MAX_WORDS];
    
    printf("Enter a sentence (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int input_len = safe_strlen(input, sizeof(input));
    if (input_len <= 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    int word_count = parse_sentence(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        fprintf(stderr, "No valid words found\n");
        return 1;
    }
    
    printf("Original words: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i].text);
    }
    printf("\n");
    
    capitalize_words(words, word_count);
    
    printf("Capitalized: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i].text);
    }
    printf("\n");
    
    reverse_words(words, word_count);
    
    printf("Reversed: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i].text);
    }
    printf("\n");
    
    return 0;
}