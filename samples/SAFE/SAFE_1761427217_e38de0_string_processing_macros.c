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

int split_into_words(const char *input, WordInfo words[], int max_words) {
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
        
        while (pos < input_len && !IS_WHITESPACE(input[pos]) && word_len < MAX_WORD_LEN - 1) {
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

void capitalize_words(WordInfo words[], int word_count) {
    for (int i = 0; i < word_count; i++) {
        if (words[i].length > 0 && islower(words[i].word[0])) {
            words[i].word[0] = toupper(words[i].word[0]);
        }
    }
}

void reverse_words(WordInfo words[], int word_count) {
    for (int i = 0; i < word_count / 2; i++) {
        WordInfo temp = words[i];
        words[i] = words[word_count - 1 - i];
        words[word_count - 1 - i] = temp;
    }
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
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int word_count = split_into_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("Original words (%d): ", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i].word);
    }
    printf("\n");
    
    capitalize_words(words, word_count);
    
    printf("Capitalized: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i].word);
    }
    printf("\n");
    
    reverse_words(words, word_count);
    
    printf("Reversed: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i].word);
    }
    printf("\n");
    
    return 0;
}