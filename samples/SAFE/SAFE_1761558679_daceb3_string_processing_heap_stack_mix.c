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
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

char *safe_strdup(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    if (len > MAX_INPUT_LEN) return NULL;
    char *new_str = malloc(len + 1);
    if (new_str == NULL) return NULL;
    strcpy(new_str, str);
    return new_str;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

int process_words(const char *input, struct WordInfo *words, int max_words) {
    if (input == NULL || words == NULL || max_words <= 0) return 0;
    
    char buffer[MAX_INPUT_LEN + 1];
    if (strlen(input) >= sizeof(buffer)) return 0;
    strcpy(buffer, input);
    
    int word_count = 0;
    char *ptr = buffer;
    
    while (*ptr && word_count < max_words) {
        while (*ptr && !is_word_char(*ptr)) ptr++;
        if (!*ptr) break;
        
        char *word_start = ptr;
        while (*ptr && is_word_char(*ptr)) ptr++;
        char temp = *ptr;
        *ptr = '\0';
        
        int found = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i].word, word_start) == 0) {
                words[i].count++;
                found = 1;
                break;
            }
        }
        
        if (!found && word_count < max_words) {
            words[word_count].word = safe_strdup(word_start);
            if (words[word_count].word == NULL) {
                *ptr = temp;
                return word_count;
            }
            words[word_count].count = 1;
            word_count++;
        }
        
        *ptr = temp;
        if (*ptr) ptr++;
    }
    
    return word_count;
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
    return wb->count - wa->count;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordInfo words[MAX_WORDS];
    
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
    
    int word_count = process_words(input, words, MAX_WORDS);
    if (word_count < 0) {
        printf("Error processing words\n");
        return 1;
    }
    
    qsort(words, word_count, sizeof(struct WordInfo), compare_word_info);
    
    printf("\nWord frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    cleanup_words(words, word_count);
    return 0;
}