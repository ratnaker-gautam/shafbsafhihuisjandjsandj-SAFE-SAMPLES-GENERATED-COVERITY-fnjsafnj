//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))

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
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)input[i])) return 0;
    }
    return 1;
}

int extract_words(const char *input, char words[][MAX_WORD_LEN], int max_words) {
    int word_count = 0;
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len && word_count < max_words; i++) {
        char c = input[i];
        if (IS_WHITESPACE(c) || c == '\0') {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN) {
                    for (size_t j = 0; j < word_len; j++) {
                        words[word_count][j] = TO_LOWER(input[word_start + j]);
                    }
                    words[word_count][word_len] = '\0';
                    word_count++;
                }
                in_word = 0;
            }
        } else {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        }
    }
    return word_count;
}

int find_word_index(WordCount *word_counts, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(word_counts[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void count_word_frequency(char words[][MAX_WORD_LEN], int word_count, WordCount *word_counts, int *unique_count) {
    *unique_count = 0;
    for (int i = 0; i < word_count; i++) {
        int index = find_word_index(word_counts, *unique_count, words[i]);
        if (index >= 0) {
            if (word_counts[index].count < INT_MAX) {
                word_counts[index].count++;
            }
        } else if (*unique_count < MAX_WORDS) {
            strncpy(word_counts[*unique_count].word, words[i], MAX_WORD_LEN - 1);
            word_counts[*unique_count].word[MAX_WORD_LEN - 1] = '\0';
            word_counts[*unique_count].count = 1;
            (*unique_count)++;
        }
    }
}

int compare_word_counts(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wb->count != wa->count) {
        if (wb->count > wa->count) return 1;
        if (wb->count < wa->count) return -1;
        return 0;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN];
    WordCount word_counts[MAX_WORDS];
    int word_count, unique_count;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = safe_strlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    word_count = extract_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    count_word_frequency(words, word_count, word_counts, &unique_count);
    
    qsort(word_counts, unique_count, sizeof(WordCount), compare_word_counts);
    
    printf("\nWord frequency (sorted by count):\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", word_counts[i].word, word_counts