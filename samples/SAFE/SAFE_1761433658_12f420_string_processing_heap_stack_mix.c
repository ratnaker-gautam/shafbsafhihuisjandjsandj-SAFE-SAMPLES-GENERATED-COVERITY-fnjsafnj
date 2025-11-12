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
    return isalnum(c) || c == '\'' || c == '-';
}

int process_words(const char *input, struct WordInfo *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return 0;
    
    char *lower_input = to_lowercase(input);
    if (lower_input == NULL) return 0;
    
    int count = 0;
    char *copy = lower_input;
    char *token = copy;
    
    while (*copy && count < MAX_WORDS) {
        while (*copy && !is_word_char(*copy)) copy++;
        if (!*copy) break;
        
        token = copy;
        while (*copy && is_word_char(*copy)) copy++;
        
        size_t word_len = copy - token;
        if (word_len > 0) {
            char *word = malloc(word_len + 1);
            if (word == NULL) {
                free(lower_input);
                return 0;
            }
            strncpy(word, token, word_len);
            word[word_len] = '\0';
            
            int found = 0;
            for (int i = 0; i < count; i++) {
                if (strcmp(words[i].word, word) == 0) {
                    words[i].count++;
                    found = 1;
                    free(word);
                    break;
                }
            }
            
            if (!found) {
                words[count].word = word;
                words[count].count = 1;
                count++;
            }
        }
        
        if (*copy) copy++;
    }
    
    free(lower_input);
    *word_count = count;
    return 1;
}

void cleanup_words(struct WordInfo *words, int count) {
    if (words == NULL) return;
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
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!process_words(input, words, &word_count)) {
        printf("Error processing words\n");
        return 1;
    }
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(struct WordInfo), compare_word_info);
        
        printf("\nWord frequency analysis:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    } else {
        printf("No words found in input\n");
    }
    
    cleanup_words(words, word_count);
    return 0;
}