//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_WORD_LEN 50
#define MAX_LINE_LEN 256
#define MAX_WORDS 1000

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int extract_words(const char *text, struct WordCount words[], int max_words) {
    if (text == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    char buffer[MAX_LINE_LEN];
    if (strlen(text) >= MAX_LINE_LEN) {
        return 0;
    }
    strncpy(buffer, text, MAX_LINE_LEN - 1);
    buffer[MAX_LINE_LEN - 1] = '\0';
    
    int word_count = 0;
    char *token = strtok(buffer, " \t\n\r.,;:!?\"()[]{}");
    
    while (token != NULL && word_count < max_words) {
        size_t len = strlen(token);
        if (len > 0 && len < MAX_WORD_LEN) {
            int valid = 1;
            for (size_t i = 0; i < len; i++) {
                if (!is_valid_char(token[i])) {
                    valid = 0;
                    break;
                }
            }
            
            if (valid) {
                for (size_t i = 0; i < len; i++) {
                    token[i] = tolower((unsigned char)token[i]);
                }
                
                int found = 0;
                for (int i = 0; i < word_count; i++) {
                    if (strcmp(words[i].word, token) == 0) {
                        words[i].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found) {
                    strncpy(words[word_count].word, token, MAX_WORD_LEN - 1);
                    words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
        token = strtok(NULL, " \t\n\r.,;:!?\"()[]{}");
    }
    
    return word_count;
}

void sort_words(struct WordCount words[], int count) {
    if (words == NULL || count <= 1) {
        return;
    }
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && strcmp(words[j].word, words[j + 1].word) > 0)) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

void print_word_frequencies(struct WordCount words[], int count) {
    if (words == NULL || count <= 0) {
        return;
    }
    
    printf("Word frequencies (sorted by count, then alphabetically):\n");
    printf("--------------------------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        if (words[i].count > 0) {
            printf("%-15s: %d\n", words[i].word, words[i].count);
        }
    }
}

int main(void) {
    char input_text[MAX_LINE_LEN * 10];
    char buffer[MAX_LINE_LEN];
    
    printf("Enter text (max %d characters total, empty line to finish):\n", MAX_LINE_LEN * 10 - 1);
    
    input_text[0] = '\0';
    size_t total_len = 0;
    
    while (fgets(buffer, MAX_LINE_LEN, stdin) != NULL) {
        if (buffer[0] == '\n' || buffer[0] == '\0') {
            break;
        }
        
        size_t buffer_len = strlen(buffer);
        if (total_len + buffer_len >= MAX_LINE_LEN * 10 - 1) {
            printf("Input too long. Processing what was received.\n");
            break;
        }
        
        strcat(input_text, buffer);
        total_len += buffer_len;
    }
    
    if (strlen(input_text) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    struct WordCount words[MAX_WORDS];
    int word_count = extract_words(input_text, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No valid words found in the input.\n");
        return 1;
    }
    
    sort_words(words, word_count);
    print_word_frequencies(words, word_count);
    
    return 0;
}