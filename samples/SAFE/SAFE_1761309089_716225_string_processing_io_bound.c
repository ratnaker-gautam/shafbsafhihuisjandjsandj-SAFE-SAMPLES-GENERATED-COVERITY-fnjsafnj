//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_LEN 64
#define MAX_WORDS 100

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_' || c == '-';
}

void to_lower_case(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(struct WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void process_text(const char *text, struct WordCount *words, int *word_count) {
    char buffer[MAX_INPUT_LEN];
    char temp_word[MAX_WORD_LEN];
    int buf_len = strlen(text);
    
    if (buf_len >= MAX_INPUT_LEN - 1) {
        return;
    }
    
    strncpy(buffer, text, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    int i = 0;
    while (i < buf_len && *word_count < MAX_WORDS) {
        while (i < buf_len && !is_word_char(buffer[i])) {
            i++;
        }
        
        if (i >= buf_len) break;
        
        int j = 0;
        while (i < buf_len && j < MAX_WORD_LEN - 1 && is_word_char(buffer[i])) {
            temp_word[j++] = buffer[i++];
        }
        temp_word[j] = '\0';
        
        if (j > 0) {
            to_lower_case(temp_word);
            
            int idx = find_word_index(words, *word_count, temp_word);
            if (idx >= 0) {
                words[idx].count++;
            } else {
                if (*word_count < MAX_WORDS) {
                    strncpy(words[*word_count].word, temp_word, MAX_WORD_LEN - 1);
                    words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
            }
        }
    }
}

void print_word_counts(struct WordCount *words, int count) {
    printf("\nWord frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis (max %d characters):\n", MAX_INPUT_LEN - 1);
    printf("Type 'END' on a separate line to finish input.\n\n");
    
    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "END") == 0) {
            break;
        }
        
        if (strlen(input) > 0) {
            process_text(input, words, &word_count);
        }
        
        if (word_count >= MAX_WORDS) {
            printf("Maximum word count reached. Processing complete.\n");
            break;
        }
    }
    
    if (word_count > 0) {
        print_word_counts(words, word_count);
    } else {
        printf("No words found in the input.\n");
    }
    
    return 0;
}