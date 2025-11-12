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

size_t safe_strlen(const char *str, size_t max_len) {
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
        if (!isprint(input[i]) && !IS_WHITESPACE(input[i])) {
            return 0;
        }
    }
    return 1;
}

void to_lower_case(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; i < MAX_WORD_LEN && str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
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
    size_t word_len = safe_strlen(word, MAX_WORD_LEN);
    if (word_len == 0 || word_len >= MAX_WORD_LEN) return 0;
    
    strncpy(words[*count].word, word, MAX_WORD_LEN - 1);
    words[*count].word[MAX_WORD_LEN - 1] = '\0';
    to_lower_case(words[*count].word);
    words[*count].count = 1;
    (*count)++;
    return 1;
}

void process_text(const char *input, WordCount *words, int *word_count) {
    *word_count = 0;
    if (!is_valid_input(input)) return;
    
    char buffer[MAX_INPUT_LEN + 1];
    strncpy(buffer, input, MAX_INPUT_LEN);
    buffer[MAX_INPUT_LEN] = '\0';
    
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= safe_strlen(buffer, MAX_INPUT_LEN); i++) {
        char c = buffer[i];
        
        if (isalnum(c) || c == '\'' || c == '-') {
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = c;
            }
            in_word = 1;
        } else if (IS_WHITESPACE(c) || IS_TERMINATOR(c) || c == '\0') {
            if (in_word && word_len > 0) {
                current_word[word_len] = '\0';
                int index = find_word_index(words, *word_count, current_word);
                if (index >= 0) {
                    words[index].count++;
                } else {
                    if (!add_word(words, word_count, current_word)) {
                        break;
                    }
                }
                word_len = 0;
            }
            in_word = 0;
        }
    }
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wb->count != wa->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

void print_word_counts(WordCount *words, int count) {
    if (count == 0) {
        printf("No words found.\n");
        return;
    }
    
    qsort(words, count, sizeof(WordCount), compare_word_count);
    
    printf("Word frequency analysis:\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < count; i++) {
        printf("%-20s %d\n", words[i].word, words[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis (max %d characters):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = safe_strlen(input, MAX_INPUT_L