//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE_LEN 256
#define MAX_WORD_LEN 64
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
    size_t text_len = strlen(text);
    if (text_len >= MAX_LINE_LEN) {
        return 0;
    }
    
    strncpy(buffer, text, MAX_LINE_LEN - 1);
    buffer[MAX_LINE_LEN - 1] = '\0';
    
    int word_count = 0;
    char *ptr = buffer;
    
    while (*ptr && word_count < max_words) {
        while (*ptr && !is_valid_char(*ptr)) {
            ptr++;
        }
        
        if (!*ptr) break;
        
        char *word_start = ptr;
        while (*ptr && is_valid_char(*ptr)) {
            ptr++;
        }
        
        size_t word_len = ptr - word_start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN];
            strncpy(temp_word, word_start, word_len);
            temp_word[word_len] = '\0';
            
            for (char *p = temp_word; *p; p++) {
                *p = tolower((unsigned char)*p);
            }
            
            int found = 0;
            for (int i = 0; i < word_count; i++) {
                if (strcmp(words[i].word, temp_word) == 0) {
                    words[i].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                strncpy(words[word_count].word, temp_word, MAX_WORD_LEN - 1);
                words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                words[word_count].count = 1;
                word_count++;
            }
        }
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

int get_user_input(char *buffer, size_t max_len) {
    if (buffer == NULL || max_len == 0) {
        return 0;
    }
    
    printf("Enter text (max %zu characters): ", max_len - 1);
    
    if (fgets(buffer, (int)max_len, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else if (len == max_len - 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    return 1;
}

int main(void) {
    char input_text[MAX_LINE_LEN];
    struct WordCount words[MAX_WORDS];
    
    if (!get_user_input(input_text, sizeof(input_text))) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (strlen(input_text) == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    int num_words = extract_words(input_text, words, MAX_WORDS);
    
    if (num_words == 0) {
        printf("No valid words found in input\n");
        return 0;
    }
    
    sort_words(words, num_words);
    
    printf("\nWord frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < num_words && i < 20; i++) {
        printf("%-20s : %d\n", words[i].word, words[i].count);
    }
    
    if (num_words > 20) {
        printf("... and %d more words\n", num_words - 20);
    }
    
    return 0;
}