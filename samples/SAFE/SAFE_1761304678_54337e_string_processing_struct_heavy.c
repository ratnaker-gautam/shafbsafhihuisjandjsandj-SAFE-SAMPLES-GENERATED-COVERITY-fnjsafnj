//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_chars;
    int total_words;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_chars = 0;
    analyzer->total_words = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].word[0] = '\0';
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_' || c == '-';
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= MAX_WORDS) {
        return 0;
    }
    
    size_t len = strlen(word);
    if (len >= MAX_WORD_LEN) {
        return 0;
    }
    
    int index = find_word_index(analyzer, word);
    if (index >= 0) {
        analyzer->words[index].count++;
        return 1;
    }
    
    strncpy(analyzer->words[analyzer->word_count].word, word, MAX_WORD_LEN - 1);
    analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
    analyzer->words[analyzer->word_count].count = 1;
    analyzer->words[analyzer->word_count].length = (int)len;
    analyzer->word_count++;
    return 1;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) {
        return;
    }
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    
    for (size_t i = 0; text[i] != '\0'; i++) {
        analyzer->total_chars++;
        
        if (is_word_char(text[i])) {
            if (!in_word) {
                in_word = 1;
                buf_pos = 0;
            }
            
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = (char)tolower((unsigned char)text[i]);
            }
        } else {
            if (in_word) {
                in_word = 0;
                buffer[buf_pos] = '\0';
                if (buf_pos > 0) {
                    analyzer->total_words++;
                    add_word(analyzer, buffer);
                }
            }
        }
    }
    
    if (in_word && buf_pos > 0) {
        buffer[buf_pos] = '\0';
        analyzer->total_words++;
        add_word(analyzer, buffer);
    }
}

void print_stats(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total characters: %d\n", analyzer->total_chars);
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("%-20s: %d occurrences\n", 
               analyzer->words[i].word, 
               analyzer->words[i].count);
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_buffer[4096];
    printf("Enter text to analyze (max 4095 characters):\n");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input_buffer[0] == '\n')) {
        printf("No input provided.\n");
        return 1;
    }
    
    process_text(&analyzer, input_buffer);
    print_stats(&analyzer);
    
    return 0;
}