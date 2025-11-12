//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

typedef struct {
    char word[MAX_WORD_LEN];
    int length;
} WordInfo;

int split_into_words(const char *input, WordInfo words[], int max_words) {
    if (input == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    int word_count = 0;
    int i = 0;
    
    while (input[i] != '\0' && word_count < max_words) {
        while (IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (input[i] == '\0') {
            break;
        }
        
        int j = 0;
        while (input[i] != '\0' && !IS_WHITESPACE(input[i]) && j < MAX_WORD_LEN - 1) {
            words[word_count].word[j] = input[i];
            i++;
            j++;
        }
        words[word_count].word[j] = '\0';
        words[word_count].length = j;
        word_count++;
    }
    
    return word_count;
}

void process_words(WordInfo words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return;
    }
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].length > 0) {
            words[i].word[0] = TO_UPPER(words[i].word[0]);
            
            for (int j = 1; j < words[i].length; j++) {
                words[i].word[j] = TO_LOWER(words[i].word[j]);
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
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordInfo words[MAX_WORDS];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int word_count = split_into_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("Original words: ");
    print_words(words, word_count);
    
    process_words(words, word_count);
    
    printf("Processed words: ");
    print_words(words, word_count);
    
    return 0;
}