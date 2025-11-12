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
    int is_capitalized;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    int longest_word_length;
    int shortest_word_length;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->longest_word_length = 0;
    analyzer->shortest_word_length = MAX_WORD_LEN;
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
        analyzer->words[i].word[0] = '\0';
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

void process_word(struct TextAnalyzer *analyzer, char *word) {
    if (analyzer->word_count >= MAX_WORDS || strlen(word) >= MAX_WORD_LEN) {
        return;
    }

    int length = strlen(word);
    if (length == 0) return;

    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            analyzer->words[i].count++;
            return;
        }
    }

    if (analyzer->word_count < MAX_WORDS) {
        strncpy(analyzer->words[analyzer->word_count].word, word, MAX_WORD_LEN - 1);
        analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].length = length;
        analyzer->words[analyzer->word_count].is_capitalized = isupper((unsigned char)word[0]);
        
        if (length > analyzer->longest_word_length) {
            analyzer->longest_word_length = length;
        }
        if (length < analyzer->shortest_word_length) {
            analyzer->shortest_word_length = length;
        }
        
        analyzer->word_count++;
    }
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;
    
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    int text_len = strlen(text);
    
    for (int i = 0; i < text_len && buf_index < MAX_WORD_LEN - 1; i++) {
        if (is_word_char(text[i])) {
            buffer[buf_index++] = text[i];
        } else {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                process_word(analyzer, buffer);
                analyzer->total_words++;
                buf_index = 0;
            }
        }
    }
    
    if (buf_index > 0) {
        buffer[buf_index] = '\0';
        process_word(analyzer, buffer);
        analyzer->total_words++;
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words processed: %d\n", analyzer->total_words);
    printf("Unique words found: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].count > 0) {
            printf("'%s': %d occurrences", analyzer->words[i].word, analyzer->words[i].count);
            if (analyzer->words[i].is_capitalized) {
                printf(" (capitalized)");
            }
            printf("\n");
        }
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_buffer[4096];
    printf("Enter text to analyze (max 4095 characters): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }
        
        if (strlen(input_buffer) > 0) {
            process_text(&analyzer, input_buffer);
            print_analysis(&analyzer);
        } else {
            printf("No input provided.\n");
        }
    } else {
        printf("Error reading input.\n");
        return 1;
    }
    
    return 0;
}