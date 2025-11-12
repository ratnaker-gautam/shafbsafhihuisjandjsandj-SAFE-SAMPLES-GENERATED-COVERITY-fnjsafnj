//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_UPPER(c) ((c) >= 'a' && (c) <= 'z' ? (c) - 'a' + 'A' : (c))

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
        int j = 0;
        while (i < input_len && !IS_WHITESPACE(input[i]) && j < MAX_WORD_LEN - 1) {
            words[word_count].word[j] = input[i];
            j++;
            i++;
        }
        words[word_count].word[j] = '\0';
        words[word_count].length = j;
        word_count++;
    }
    
    return word_count;
}

void process_word(char *word, int length) {
    for (int i = 0; i < length; i++) {
        word[i] = TO_UPPER(word[i]);
    }
}

int compare_words(const void *a, const void *b) {
    const WordInfo *word_a = (const WordInfo *)a;
    const WordInfo *word_b = (const WordInfo *)b;
    return strcmp(word_a->word, word_b->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordInfo words[MAX_WORDS];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
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
    
    for (int i = 0; i < word_count; i++) {
        process_word(words[i].word, words[i].length);
    }
    
    qsort(words, word_count, sizeof(WordInfo), compare_words);
    
    printf("Processed and sorted words:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i].word);
    }
    
    return 0;
}