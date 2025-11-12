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
        
        while (pos < input_len && word_len < (MAX_WORD_LEN - 1) && 
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

void capitalize_words(WordInfo words[], int count) {
    if (words == NULL || count <= 0) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        if (words[i].length > 0 && islower(words[i].word[0])) {
            words[i].word[0] = toupper(words[i].word[0]);
        }
    }
}

void reverse_words(WordInfo words[], int count) {
    if (words == NULL || count <= 0) {
        return;
    }
    
    for (int i = 0; i < count / 2; i++) {
        WordInfo temp = words[i];
        words[i] = words[count - 1 - i];
        words[count - 1 - i] = temp;
    }
}

int build_output(const WordInfo words[], int count, char *output, size_t output_size) {
    if (words == NULL || output == NULL || output_size == 0 || count <= 0) {
        return 0;
    }
    
    size_t pos = 0;
    
    for (int i = 0; i < count && pos < output_size - 1; i++) {
        int word_len = safe_strlen(words[i].word, MAX_WORD_LEN);
        
        if (pos + word_len + 1 >= output_size) {
            break;
        }
        
        if (i > 0) {
            output[pos++] = ' ';
        }
        
        for (int j = 0; j < word_len && pos < output_size - 1; j++) {
            output[pos++] = words[i].word[j];
        }
    }
    
    if (pos < output_size - 1) {
        output[pos++] = '.';
    }
    
    output[pos] = '\0';
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char output[MAX_INPUT_LEN + 1];
    WordInfo words[MAX_WORDS];
    
    printf("Enter a sentence (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int input_len = safe_strlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (safe_strlen(input, sizeof(input)) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int word_count = parse_sentence(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        fprintf(stderr, "No valid words found\n");
        return 1;
    }
    
    capitalize_words(words, word_count);
    reverse_words(words, word_count);
    
    if (!build_output(words, word_count, output, sizeof(output))) {
        fprintf(stderr, "Error building output\n");
        return 1;
    }
    
    printf("Processed sentence: %s\n", output);
    
    return 0;
}