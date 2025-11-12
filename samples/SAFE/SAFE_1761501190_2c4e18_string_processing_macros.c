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

int split_into_words(const char *input, WordInfo words[], int max_words) {
    if (input == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    int word_count = 0;
    int input_len = strlen(input);
    int i = 0;
    
    while (i < input_len && word_count < max_words) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        int word_start = i;
        while (i < input_len && !IS_WHITESPACE(input[i]) && !IS_TERMINATOR(input[i])) {
            i++;
        }
        
        int word_length = i - word_start;
        if (word_length > 0 && word_length < MAX_WORD_LEN) {
            strncpy(words[word_count].word, &input[word_start], word_length);
            words[word_count].word[word_length] = '\0';
            words[word_count].length = word_length;
            word_count++;
        }
        
        if (i < input_len && IS_TERMINATOR(input[i])) {
            i++;
        }
    }
    
    return word_count;
}

void capitalize_words(WordInfo words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return;
    }
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].length > 0 && islower(words[i].word[0])) {
            words[i].word[0] = toupper(words[i].word[0]);
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
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int word_count = split_into_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        fprintf(stderr, "No valid words found\n");
        return 1;
    }
    
    capitalize_words(words, word_count);
    
    printf("Capitalized sentence: ");
    print_words(words, word_count);
    
    return 0;
}