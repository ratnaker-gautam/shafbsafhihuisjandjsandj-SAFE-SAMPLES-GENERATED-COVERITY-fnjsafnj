//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

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

int is_valid_char(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '\'' || c == '-';
}

int find_word(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void process_text(const char *text, WordCount *words, int *word_count) {
    int len = safe_strlen(text, MAX_INPUT_LEN);
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    
    for (int i = 0; i <= len; i++) {
        char c = text[i];
        
        if (is_valid_char(c)) {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = TO_LOWER(c);
            }
        } else {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                
                int existing = find_word(words, *word_count, buffer);
                if (existing >= 0) {
                    words[existing].count++;
                } else if (*word_count < MAX_WORDS) {
                    strncpy(words[*word_count].word, buffer, MAX_WORD_LEN - 1);
                    words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
                buf_pos = 0;
            }
        }
    }
}

int compare_words(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strnlen(input, sizeof(input));
    if (input_len == 0 || (input_len == sizeof(input) - 1 && input[sizeof(input) - 2] != '\n')) {
        fprintf(stderr, "Input too long or empty\n");
        return 1;
    }
    
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    process_text(input, words, &word_count);
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return 0;
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_words);
    
    printf("\nWord frequency analysis:\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-20s %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}