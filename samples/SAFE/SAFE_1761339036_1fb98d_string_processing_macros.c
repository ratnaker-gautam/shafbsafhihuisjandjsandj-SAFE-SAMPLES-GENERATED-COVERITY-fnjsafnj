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
    return (int)len;
}

int parse_words(const char *input, Word *words, int max_words) {
    int word_count = 0;
    int i = 0;
    int input_len = safe_strlen(input, MAX_INPUT_LEN);
    
    while (i < input_len && word_count < max_words) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        int word_start = i;
        int word_len = 0;
        
        while (i < input_len && !IS_WHITESPACE(input[i]) && word_len < MAX_WORD_LEN - 1) {
            word_len++;
            i++;
        }
        
        if (word_len > 0) {
            int copy_len = word_len;
            if (copy_len >= MAX_WORD_LEN) {
                copy_len = MAX_WORD_LEN - 1;
            }
            
            for (int j = 0; j < copy_len; j++) {
                words[word_count].text[j] = input[word_start + j];
            }
            words[word_count].text[copy_len] = '\0';
            words[word_count].length = copy_len;
            word_count++;
        }
    }
    
    return word_count;
}

void capitalize_words(Word *words, int word_count) {
    for (int i = 0; i < word_count; i++) {
        if (words[i].length > 0 && islower(words[i].text[0])) {
            words[i].text[0] = (char)toupper(words[i].text[0]);
        }
    }
}

void build_output(const Word *words, int word_count, char *output, size_t output_size) {
    int pos = 0;
    
    for (int i = 0; i < word_count; i++) {
        int word_len = safe_strlen(words[i].text, MAX_WORD_LEN);
        
        if (pos + word_len + 2 >= output_size) {
            break;
        }
        
        if (i > 0) {
            output[pos++] = ' ';
        }
        
        for (int j = 0; j < word_len; j++) {
            output[pos++] = words[i].text[j];
        }
    }
    
    if (pos < output_size) {
        output[pos] = '\0';
    } else {
        output[output_size - 1] = '\0';
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    Word words[MAX_WORDS];
    char output[MAX_INPUT_LEN];
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int input_len = safe_strlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (input_len == 0 || (input_len == 1 && input[0] == '\0')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int word_count = parse_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        fprintf(stderr, "No words found\n");
        return 1;
    }
    
    capitalize_words(words, word_count);
    build_output(words, word_count, output, sizeof(output));
    
    printf("Processed text: %s\n", output);
    
    return 0;
}