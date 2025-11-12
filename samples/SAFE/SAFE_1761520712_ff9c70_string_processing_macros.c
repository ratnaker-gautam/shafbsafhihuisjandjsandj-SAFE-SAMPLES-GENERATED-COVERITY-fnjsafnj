//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')

typedef struct {
    char word[MAX_WORD_LEN];
    int length;
} WordInfo;

int safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int parse_sentence(const char *input, WordInfo words[], int max_words) {
    if (input == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    int word_count = 0;
    int pos = 0;
    int input_len = safe_strlen(input, MAX_INPUT_LEN);
    
    while (pos < input_len && word_count < max_words) {
        while (pos < input_len && IS_WHITESPACE(input[pos])) {
            pos++;
        }
        
        if (pos >= input_len || IS_TERMINATOR(input[pos])) {
            break;
        }
        
        int word_start = pos;
        int word_len = 0;
        
        while (pos < input_len && word_len < MAX_WORD_LEN - 1 && 
               !IS_WHITESPACE(input[pos]) && !IS_TERMINATOR(input[pos])) {
            words[word_count].word[word_len] = input[pos];
            word_len++;
            pos++;
        }
        
        if (word_len > 0) {
            words[word_count].word[word_len] = '\0';
            words[word_count].length = word_len;
            word_count++;
        }
    }
    
    return word_count;
}

void process_words(WordInfo words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return;
    }
    
    for (int i = 0; i < word_count; i++) {
        for (int j = 0; j < words[i].length; j++) {
            if (j == 0) {
                words[i].word[j] = toupper((unsigned char)words[i].word[j]);
            } else {
                words[i].word[j] = tolower((unsigned char)words[i].word[j]);
            }
        }
    }
}

void print_words(const WordInfo words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return;
    }
    
    for (int i = 0; i < word_count; i++) {
        printf("%s", words[i].word);
        if (i < word_count - 1) {
            printf(" ");
        }
    }
    printf(".\n");
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordInfo words[MAX_WORDS];
    
    printf("Enter a sentence (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (input_len == 0 || (input_len == 1 && input[0] == '\0')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int word_count = parse_sentence(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        fprintf(stderr, "No valid words found\n");
        return 1;
    }
    
    process_words(words, word_count);
    
    printf("Processed sentence: ");
    print_words(words, word_count);
    
    return 0;
}