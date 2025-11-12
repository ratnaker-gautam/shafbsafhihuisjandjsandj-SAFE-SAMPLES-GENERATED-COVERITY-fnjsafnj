//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

typedef struct {
    char *word;
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

char *strdup_safe(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    if (len == 0) return NULL;
    char *copy = malloc(len + 1);
    if (copy == NULL) return NULL;
    memcpy(copy, s, len + 1);
    return copy;
}

int compare_words(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return strcmp(wa->word, wb->word);
}

void process_string(const char *input) {
    if (input == NULL || strlen(input) == 0) {
        printf("No input to process.\n");
        return;
    }

    char buffer[MAX_INPUT_LEN];
    if (strlen(input) >= MAX_INPUT_LEN) {
        printf("Input too long.\n");
        return;
    }
    strncpy(buffer, input, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';

    WordCount words[MAX_WORDS];
    int word_count = 0;
    char temp_word[MAX_WORD_LEN];
    int temp_len = 0;
    int in_word = 0;

    for (size_t i = 0; i <= strlen(buffer); i++) {
        char c = buffer[i];
        if (is_valid_char(c) && temp_len < MAX_WORD_LEN - 1) {
            temp_word[temp_len++] = tolower((unsigned char)c);
            in_word = 1;
        } else {
            if (in_word && temp_len > 0) {
                temp_word[temp_len] = '\0';
                
                int found = 0;
                for (int j = 0; j < word_count; j++) {
                    if (strcmp(words[j].word, temp_word) == 0) {
                        words[j].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found && word_count < MAX_WORDS) {
                    char *word_copy = strdup_safe(temp_word);
                    if (word_copy != NULL) {
                        words[word_count].word = word_copy;
                        words[word_count].count = 1;
                        word_count++;
                    }
                }
                
                temp_len = 0;
                in_word = 0;
            }
        }
    }

    if (word_count > 0) {
        qsort(words, word_count, sizeof(WordCount), compare_words);
        
        printf("Word frequencies:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
        
        for (int i = 0; i < word_count; i++) {
            free(words[i].word);
        }
    } else {
        printf("No words found.\n");
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    process_string(input);
    
    return 0;
}