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
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
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
    
    char normalized[MAX_WORD_LEN];
    size_t word_len = strlen(word);
    if (word_len >= MAX_WORD_LEN) return;
    strcpy(normalized, word);
    to_lower_case(normalized);
    
    int index = find_word_index(analyzer, normalized);
    if (index >= 0) {
        analyzer->words[index].count++;
        if (is_capitalized) {
            analyzer->words[index].is_capitalized = 1;
        }
    } else {
        struct WordStats *ws = &analyzer->words[analyzer->word_count];
        strcpy(ws->word, normalized);
        ws->count = 1;
        ws->length = (int)word_len;
        ws->is_capitalized = is_capitalized;
        analyzer->word_count++;
        analyzer->unique_words++;
    }
    analyzer->total_words++;
    analyzer->total_chars += (int)word_len;
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
                in_word = 1;
                word_start_cap = isupper((unsigned char)text[i]) ? 1 : 0;
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
                buf_index = 0;
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
    printf("Average word length: %.2f\n", 
           analyzer->total_words > 0 ? (double)analyzer->total_chars / analyzer->total_words : 0.0);
    printf("\nWord frequency (top 10):\n");
    
    for (int i = 0; i < (analyzer->word_count < 10 ? analyzer->word_count : 10); i++) {
        int max_index = i;
        for (int j = i + 1; j < analyzer->word_count; j++) {
            if (analyzer->words[j].count > analyzer->words[max_index].count) {
                max_index = j;
            }
        }
        
        struct WordStats temp = analyzer->words[i];
        analyzer->words[i] = analyzer->words[max_index];
        analyzer->words[max_index] = temp;
        
        printf("%-15s: %d (length: %d%s)\n", 
               analyzer->words[i].word,
               analyzer->words[i].count,
               analyzer->words[i].length,
               analyzer->words[i].is_capitalized ? ", capitalized" : "");
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_buffer[4096];
    printf("Enter text to analyze (max 4095 characters):\n");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;