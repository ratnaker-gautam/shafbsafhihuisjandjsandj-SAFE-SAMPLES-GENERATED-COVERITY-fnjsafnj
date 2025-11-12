//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
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
    return (len < max_len) ? (int)len : -1;
}

int split_into_words(const char *input, WordInfo *words, int max_words) {
    int word_count = 0;
    int i = 0;
    int input_len = safe_strlen(input, MAX_INPUT_LEN);
    
    if (input_len < 0) {
        return -1;
    }
    
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
            words[word_count].word[word_len] = input[i];
            word_len++;
            i++;
        }
        
        if (word_len > 0) {
            words[word_count].word[word_len] = '\0';
            words[word_count].length = word_len;
            word_count++;
        }
    }
    
    return word_count;
}

void capitalize_words(WordInfo *words, int word_count) {
    for (int i = 0; i < word_count; i++) {
        if (words[i].length > 0 && isalpha((unsigned char)words[i].word[0])) {
            words[i].word[0] = toupper((unsigned char)words[i].word[0]);
        }
    }
}

void reverse_words(WordInfo *words, int word_count) {
    for (int i = 0; i < word_count / 2; i++) {
        WordInfo temp = words[i];
        words[i] = words[word_count - 1 - i];
        words[word_count - 1 - i] = temp;
    }
}

int reconstruct_sentence(const WordInfo *words, int word_count, char *output, int output_size) {
    int pos = 0;
    
    for (int i = 0; i < word_count; i++) {
        int word_len = safe_strlen(words[i].word, MAX_WORD_LEN);
        if (word_len < 0) {
            return -1;
        }
        
        if (pos + word_len + 1 >= output_size) {
            return -1;
        }
        
        if (i > 0) {
            output[pos++] = ' ';
        }
        
        for (int j = 0; j < word_len; j++) {
            output[pos++] = words[i].word[j];
        }
    }
    
    output[pos] = '\0';
    return pos;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char output[MAX_INPUT_LEN + 1];
    WordInfo words[MAX_WORDS];
    
    printf("Enter a sentence (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    int input_len = safe_strlen(input, sizeof(input));
    if (input_len < 0) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    int word_count = split_into_words(input, words, MAX_WORDS);
    if (word_count <= 0) {
        fprintf(stderr, "Error splitting words\n");
        return EXIT_FAILURE;
    }
    
    capitalize_words(words, word_count);
    reverse_words(words, word_count);
    
    int result = reconstruct_sentence(words, word_count, output, sizeof(output));
    if (result < 0) {
        fprintf(stderr, "Error reconstructing sentence\n");
        return EXIT_FAILURE;
    }
    
    printf("Processed sentence: %s\n", output);
    
    return EXIT_SUCCESS;
}