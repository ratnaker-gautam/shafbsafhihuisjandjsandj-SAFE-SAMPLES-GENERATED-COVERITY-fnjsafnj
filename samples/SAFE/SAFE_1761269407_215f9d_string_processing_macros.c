//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return 0;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 1;
}

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i]) && !IS_WHITESPACE(str[i])) {
            return 0;
        }
    }
    return 1;
}

void to_lower_case(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int add_word(WordCount *words, int *word_count, const char *word) {
    if (words == NULL || word_count == NULL || word == NULL) return 0;
    if (*word_count >= MAX_WORDS) return 0;
    
    for (int i = 0; i < *word_count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            words[i].count++;
            return 1;
        }
    }
    
    if (safe_strncpy(words[*word_count].word, word, MAX_WORD_LEN)) {
        words[*word_count].count = 1;
        (*word_count)++;
        return 1;
    }
    return 0;
}

int process_sentence(const char *sentence, WordCount *words, int *word_count) {
    if (sentence == NULL || words == NULL || word_count == NULL) return 0;
    
    char buffer[MAX_INPUT_LEN];
    if (!safe_strncpy(buffer, sentence, MAX_INPUT_LEN)) return 0;
    to_lower_case(buffer);
    
    char temp_word[MAX_WORD_LEN];
    size_t buf_len = strlen(buffer);
    size_t word_start = 0;
    size_t word_len = 0;
    
    for (size_t i = 0; i <= buf_len; i++) {
        if (i < buf_len && !IS_WHITESPACE(buffer[i]) && !IS_TERMINATOR(buffer[i])) {
            if (word_len < MAX_WORD_LEN - 1) {
                temp_word[word_len++] = buffer[i];
            }
        } else {
            if (word_len > 0) {
                temp_word[word_len] = '\0';
                if (!add_word(words, word_count, temp_word)) {
                    return 0;
                }
                word_len = 0;
            }
        }
    }
    return 1;
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter a sentence: ");
    if (fgets(input, MAX_INPUT_LEN, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (!process_sentence(input, words, &word_count)) {
        fprintf(stderr, "Error processing sentence\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_word_count);
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}