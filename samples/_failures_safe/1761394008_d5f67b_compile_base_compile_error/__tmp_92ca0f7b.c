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
    int total_chars;
    int total_words;
    int unique_words;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_chars = 0;
    analyzer->total_words = 0;
    analyzer->unique_words = 0;
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

void to_lower_case(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextAnalyzer *analyzer, const char *word, int is_capitalized) {
    if (analyzer->word_count >= MAX_WORDS) return;
    
    char temp[MAX_WORD_LEN];
    if (strlen(word) >= MAX_WORD_LEN) return;
    strncpy(temp, word, MAX_WORD_LEN - 1);
    temp[MAX_WORD_LEN - 1] = '\0';
    to_lower_case(temp);
    
    int index = find_word_index(analyzer, temp);
    if (index >= 0) {
        analyzer->words[index].count++;
    } else {
        struct WordStats *ws = &analyzer->words[analyzer->word_count];
        strncpy(ws->word, temp, MAX_WORD_LEN - 1);
        ws->word[MAX_WORD_LEN - 1] = '\0';
        ws->count = 1;
        ws->length = strlen(temp);
        ws->is_capitalized = is_capitalized;
        analyzer->word_count++;
        analyzer->unique_words++;
    }
    analyzer->total_words++;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;
    
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    int in_word = 0;
    int word_start_cap = 0;
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (is_word_char(text[i])) {
            if (!in_word) {
                word_start_cap = isupper((unsigned char)text[i]);
                in_word = 1;
                buf_index = 0;
            }
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = text[i];
            }
        } else {
            if (in_word) {
                buffer[buf_index] = '\0';
                if (buf_index > 0) {
                    add_word(analyzer, buffer, word_start_cap);
                }
                in_word = 0;
            }
        }
    }
    
    if (in_word && buf_index > 0) {
        buffer[buf_index] = '\0';
        add_word(analyzer, buffer, word_start_cap);
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("\nWord frequency (top 10):\n");
    
    for (int i = 0; i < analyzer->word_count && i < 10; i++) {
        printf("%-15s: %d occurrences\n", 
               analyzer->words[i].word, analyzer->words[i].count);
    }
    
    int total_chars = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        total_chars += analyzer->words[i].length * analyzer->words[i].count;
    }
    printf("\nAverage word length: %.2f\n", 
           analyzer->total_words > 0 ? (float)total_chars / analyzer->total_words : 0.0);
    
    int capitalized_count = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].is_capitalized) {
            capitalized_count += analyzer->words[i].count;
        }
    }
    printf("Capitalized words: %d\n", capitalized_count);
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
    if (len > 0 && input_buffer[len - 1] ==