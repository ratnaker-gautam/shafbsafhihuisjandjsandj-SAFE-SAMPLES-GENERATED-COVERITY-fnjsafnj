//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

struct WordInfo {
    char *word;
    int count;
};

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

char *to_lowercase(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    char *result = malloc(len + 1);
    if (result == NULL) return NULL;
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower(str[i]);
    }
    result[len] = '\0';
    return result;
}

int is_word_char(char c) {
    return isalnum(c) || c == '\'';
}

int find_word_index(struct WordInfo *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void free_words(struct WordInfo *words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i].word);
    }
}

int compare_word_info(const void *a, const void *b) {
    const struct WordInfo *wa = (const struct WordInfo *)a;
    const struct WordInfo *wb = (const struct WordInfo *)b;
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordInfo words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    char *lower_input = to_lowercase(input);
    if (lower_input == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char *current = lower_input;
    while (*current != '\0' && word_count < MAX_WORDS) {
        while (*current != '\0' && !is_word_char(*current)) {
            current++;
        }
        if (*current == '\0') break;
        
        char *word_start = current;
        while (*current != '\0' && is_word_char(*current)) {
            current++;
        }
        
        size_t word_len = current - word_start;
        if (word_len > 0) {
            char temp_word[256];
            if (word_len >= sizeof(temp_word)) {
                word_len = sizeof(temp_word) - 1;
            }
            strncpy(temp_word, word_start, word_len);
            temp_word[word_len] = '\0';
            
            int existing_index = find_word_index(words, word_count, temp_word);
            if (existing_index >= 0) {
                words[existing_index].count++;
            } else {
                words[word_count].word = malloc(word_len + 1);
                if (words[word_count].word == NULL) {
                    free(lower_input);
                    free_words(words, word_count);
                    fprintf(stderr, "Memory allocation failed\n");
                    return 1;
                }
                strcpy(words[word_count].word, temp_word);
                words[word_count].count = 1;
                word_count++;
            }
        }
    }
    
    qsort(words, word_count, sizeof(struct WordInfo), compare_word_info);
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    free(lower_input);
    free_words(words, word_count);
    return 0;
}