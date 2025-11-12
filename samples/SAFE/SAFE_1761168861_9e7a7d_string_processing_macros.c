//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')

typedef struct {
    char text[MAX_WORD_LEN];
    size_t length;
} Word;

size_t safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int parse_sentence(const char *input, Word words[], size_t max_words) {
    if (input == NULL || words == NULL || max_words == 0) {
        return -1;
    }
    
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len == 0 || input_len >= MAX_INPUT_LEN) {
        return -1;
    }
    
    size_t word_count = 0;
    size_t pos = 0;
    size_t word_start = 0;
    int in_word = 0;
    
    while (pos < input_len && word_count < max_words) {
        char c = input[pos];
        
        if (IS_WHITESPACE(c) || IS_TERMINATOR(c)) {
            if (in_word) {
                size_t word_len = pos - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN) {
                    for (size_t i = 0; i < word_len; i++) {
                        words[word_count].text[i] = input[word_start + i];
                    }
                    words[word_count].text[word_len] = '\0';
                    words[word_count].length = word_len;
                    word_count++;
                }
                in_word = 0;
            }
            
            if (IS_TERMINATOR(c)) {
                break;
            }
        } else if (isalpha((unsigned char)c)) {
            if (!in_word) {
                word_start = pos;
                in_word = 1;
            }
        }
        
        pos++;
    }
    
    if (in_word && word_count < max_words) {
        size_t word_len = pos - word_start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            for (size_t i = 0; i < word_len; i++) {
                words[word_count].text[i] = input[word_start + i];
            }
            words[word_count].text[word_len] = '\0';
            words[word_count].length = word_len;
            word_count++;
        }
    }
    
    return (int)word_count;
}

void process_words(Word words[], int count) {
    if (words == NULL || count <= 0) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        if (words[i].length > 0) {
            for (size_t j = 0; j < words[i].length; j++) {
                words[i].text[j] = toupper((unsigned char)words[i].text[j]);
            }
        }
    }
}

void print_words(const Word words[], int count) {
    if (words == NULL || count <= 0) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        if (words[i].length > 0) {
            printf("%s", words[i].text);
            if (i < count - 1) {
                printf(" ");
            }
        }
    }
    printf(".\n");
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    Word words[MAX_WORDS];
    
    printf("Enter a sentence (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = safe_strlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    int word_count = parse_sentence(input, words, MAX_WORDS);
    if (word_count <= 0) {
        fprintf(stderr, "No valid words found or input error\n");
        return 1;
    }
    
    printf("Original words: ");
    print_words(words, word_count);
    
    process_words(words, word_count);
    
    printf("Processed words: ");
    print_words(words, word_count);
    
    return 0;
}