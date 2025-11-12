//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int is_valid_input(const char* str) {
    if (str == NULL) return 0;
    if (strlen(str) == 0) return 0;
    if (strlen(str) >= MAX_INPUT_LEN) return 0;
    return 1;
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

int extract_words(const char* text, WordCount words[], int* word_count) {
    if (!is_valid_input(text)) return 0;
    if (words == NULL || word_count == NULL) return 0;
    
    *word_count = 0;
    int len = strlen(text);
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    
    for (int i = 0; i <= len; i++) {
        if (i < len && is_word_char(text[i])) {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = tolower((unsigned char)text[i]);
            }
        } else {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                
                int found = 0;
                for (int j = 0; j < *word_count; j++) {
                    if (strcmp(words[j].word, buffer) == 0) {
                        words[j].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found && *word_count < MAX_WORDS) {
                    if (strlen(buffer) < MAX_WORD_LEN) {
                        strcpy(words[*word_count].word, buffer);
                        words[*word_count].count = 1;
                        (*word_count)++;
                    }
                }
                
                buf_pos = 0;
            }
        }
    }
    
    return 1;
}

void sort_words(WordCount words[], int word_count) {
    if (words == NULL || word_count <= 1) return;
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (words[j].count < words[j + 1].count) {
                WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

void print_word_stats(const WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < word_count; i++) {
        if (i < 10) {
            printf("%2d. %-15s: %d\n", i + 1, words[i].word, words[i].count);
        }
    }
    
    if (word_count > 10) {
        printf("... and %d more words\n", word_count - 10);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (!extract_words(input, words, &word_count)) {
        printf("Error processing text.\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in the input.\n");
        return 0;
    }
    
    sort_words(words, word_count);
    print_word_stats(words, word_count);
    
    return 0;
}