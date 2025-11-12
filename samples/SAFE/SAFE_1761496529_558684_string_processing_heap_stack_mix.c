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

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i])) return 0;
    }
    return 1;
}

char *safe_strdup(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    char *new_str = malloc(len + 1);
    if (new_str == NULL) return NULL;
    strcpy(new_str, str);
    return new_str;
}

int compare_words(const void *a, const void *b) {
    const struct WordInfo *wa = (const struct WordInfo *)a;
    const struct WordInfo *wb = (const struct WordInfo *)b;
    return strcmp(wa->word, wb->word);
}

void process_sentence(const char *sentence, struct WordInfo *word_list, int *word_count) {
    char buffer[MAX_INPUT_LEN + 1];
    strcpy(buffer, sentence);
    
    char *token = strtok(buffer, " \t\n\r");
    while (token != NULL && *word_count < MAX_WORDS) {
        int found = 0;
        for (int i = 0; i < *word_count; i++) {
            if (strcmp(word_list[i].word, token) == 0) {
                word_list[i].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            char *word_copy = safe_strdup(token);
            if (word_copy != NULL) {
                word_list[*word_count].word = word_copy;
                word_list[*word_count].count = 1;
                (*word_count)++;
            }
        }
        
        token = strtok(NULL, " \t\n\r");
    }
}

void cleanup_word_list(struct WordInfo *word_list, int word_count) {
    for (int i = 0; i < word_count; i++) {
        free(word_list[i].word);
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    struct WordInfo word_list[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter a sentence: ");
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
    
    process_sentence(input, word_list, &word_count);
    
    if (word_count > 0) {
        qsort(word_list, word_count, sizeof(struct WordInfo), compare_words);
        
        printf("Word frequencies:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %d\n", word_list[i].word, word_list[i].count);
        }
    } else {
        printf("No words found\n");
    }
    
    cleanup_word_list(word_list, word_count);
    
    return 0;
}