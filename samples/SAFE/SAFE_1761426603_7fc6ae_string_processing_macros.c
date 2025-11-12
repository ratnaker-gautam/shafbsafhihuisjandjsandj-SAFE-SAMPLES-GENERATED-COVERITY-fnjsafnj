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
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = safe_strlen(input, MAX_INPUT_LEN + 1);
    if (len > MAX_INPUT_LEN) return 0;
    return 1;
}

int find_or_add_word(WordCount *words, int *word_count, const char *word) {
    for (int i = 0; i < *word_count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            words[i].count++;
            return 1;
        }
    }
    if (*word_count >= MAX_WORDS) return 0;
    size_t word_len = safe_strlen(word, MAX_WORD_LEN);
    if (word_len >= MAX_WORD_LEN) return 0;
    strcpy(words[*word_count].word, word);
    words[*word_count].count = 1;
    (*word_count)++;
    return 1;
}

void process_sentence(const char *sentence, WordCount *words, int *word_count) {
    const char *ptr = sentence;
    char current_word[MAX_WORD_LEN];
    int word_idx = 0;
    
    while (*ptr != '\0' && word_idx < MAX_WORD_LEN - 1) {
        if (IS_WHITESPACE(*ptr)) {
            if (word_idx > 0) {
                current_word[word_idx] = '\0';
                find_or_add_word(words, word_count, current_word);
                word_idx = 0;
            }
        } else if (isalpha((unsigned char)*ptr)) {
            current_word[word_idx++] = tolower((unsigned char)*ptr);
        }
        ptr++;
    }
    
    if (word_idx > 0) {
        current_word[word_idx] = '\0';
        find_or_add_word(words, word_count, current_word);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = safe_strlen(input, sizeof(input));
    if (input_len == 0 || !is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    const char *ptr = input;
    char sentence[MAX_INPUT_LEN + 1];
    int sent_idx = 0;
    
    while (*ptr != '\0' && sent_idx < MAX_INPUT_LEN) {
        if (IS_TERMINATOR(*ptr)) {
            sentence[sent_idx++] = *ptr;
            sentence[sent_idx] = '\0';
            process_sentence(sentence, words, &word_count);
            sent_idx = 0;
        } else {
            sentence[sent_idx++] = *ptr;
        }
        ptr++;
    }
    
    if (sent_idx > 0) {
        sentence[sent_idx] = '\0';
        process_sentence(sentence, words, &word_count);
    }
    
    printf("\nWord frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}