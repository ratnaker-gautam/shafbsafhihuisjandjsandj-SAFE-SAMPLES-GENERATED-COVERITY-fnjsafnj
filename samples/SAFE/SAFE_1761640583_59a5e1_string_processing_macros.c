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
        return -1;
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

void capitalize_words(WordInfo words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return;
    }
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].length > 0 && isalpha((unsigned char)words[i].word[0])) {
            words[i].word[0] = toupper((unsigned char)words[i].word[0]);
        }
    }
}

void reverse_words(WordInfo words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return;
    }
    
    for (int i = 0; i < word_count / 2; i++) {
        WordInfo temp = words[i];
        words[i] = words[word_count - 1 - i];
        words[word_count - 1 - i] = temp;
    }
}

int reconstruct_sentence(const WordInfo words[], int word_count, char *output, int output_size) {
    if (words == NULL || output == NULL || output_size <= 0 || word_count <= 0) {
        return -1;
    }
    
    int pos = 0;
    
    for (int i = 0; i < word_count; i++) {
        int word_len = strlen(words[i].word);
        
        if (pos + word_len + 1 >= output_size) {
            return -1;
        }
        
        if (i > 0) {
            output[pos++] = ' ';
        }
        
        strncpy(output + pos, words[i].word, word_len);
        pos += word_len;
    }
    
    output[pos] = '\0';
    return 0;
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
    if (word_count <= 0) {
        fprintf(stderr, "No words found or error processing\n");
        return 1;
    }
    
    printf("Original words: %d\n", word_count);
    
    capitalize_words(words, word_count);
    
    reverse_words(words, word_count);
    
    if (reconstruct_sentence(words, word_count, output, sizeof(output)) != 0) {
        fprintf(stderr, "Error reconstructing sentence\n");
        return 1;
    }
    
    printf("Processed sentence: %s\n", output);
    
    return 0;
}