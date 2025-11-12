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
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

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

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = safe_strlen(str, MAX_INPUT_LEN + 1);
    return len > 0 && len <= MAX_INPUT_LEN;
}

void normalize_word(char *dest, const char *src, size_t max_len) {
    size_t i = 0;
    size_t j = 0;
    while (src[i] != '\0' && j < max_len - 1) {
        if (isalnum((unsigned char)src[i])) {
            dest[j] = TO_LOWER(src[i]);
            j++;
        }
        i++;
    }
    dest[j] = '\0';
}

int find_word_index(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(WordCount *words, int *count, const char *word) {
    if (*count >= MAX_WORDS) return 0;
    
    char normalized[MAX_WORD_LEN];
    normalize_word(normalized, word, MAX_WORD_LEN);
    
    if (safe_strlen(normalized, MAX_WORD_LEN) == 0) return 1;
    
    int index = find_word_index(words, *count, normalized);
    if (index >= 0) {
        words[index].count++;
    } else {
        size_t len = safe_strlen(normalized, MAX_WORD_LEN);
        if (len >= MAX_WORD_LEN) return 0;
        strncpy(words[*count].word, normalized, MAX_WORD_LEN);
        words[*count].word[MAX_WORD_LEN - 1] = '\0';
        words[*count].count = 1;
        (*count)++;
    }
    return 1;
}

void process_text(const char *text, WordCount *words, int *word_count) {
    *word_count = 0;
    if (!is_valid_input(text)) return;
    
    char buffer[MAX_INPUT_LEN + 1];
    size_t text_len = safe_strlen(text, MAX_INPUT_LEN);
    strncpy(buffer, text, MAX_INPUT_LEN);
    buffer[text_len] = '\0';
    
    size_t i = 0;
    char current_word[MAX_WORD_LEN];
    size_t word_pos = 0;
    
    while (i < text_len && word_pos < MAX_WORD_LEN - 1) {
        if (IS_WHITESPACE(buffer[i]) || IS_TERMINATOR(buffer[i])) {
            if (word_pos > 0) {
                current_word[word_pos] = '\0';
                add_word(words, word_count, current_word);
                word_pos = 0;
            }
        } else {
            current_word[word_pos] = buffer[i];
            word_pos++;
        }
        i++;
    }
    
    if (word_pos > 0) {
        current_word[word_pos] = '\0';
        add_word(words, word_count, current_word);
    }
}

int compare_word_counts(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wb->count != wa->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

void print_word_counts(WordCount *words, int count) {
    if (count <= 0) {
        printf("No words found.\n");
        return;
    }
    
    qsort(words, count, sizeof(WordCount), compare_word_counts);
    
    printf("Word frequency analysis:\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < count; i++) {
        if (words[i].count > 0) {
            printf("%-20s %d\n", words[i].word, words[i].count);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words