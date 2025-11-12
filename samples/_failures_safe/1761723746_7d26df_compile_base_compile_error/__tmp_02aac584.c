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
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].word[0] = '\0';
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextAnalyzer *analyzer, const char *word, int is_capitalized) {
    if (analyzer->word_count >= MAX_WORDS) {
        return -1;
    }
    
    int index = find_word_index(analyzer, word);
    if (index >= 0) {
        analyzer->words[index].count++;
        return index;
    }
    
    size_t len = strlen(word);
    if (len >= MAX_WORD_LEN) {
        return -1;
    }
    
    strncpy(analyzer->words[analyzer->word_count].word, word, MAX_WORD_LEN - 1);
    analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
    analyzer->words[analyzer->word_count].count = 1;
    analyzer->words[analyzer->word_count].length = (int)len;
    analyzer->words[analyzer->word_count].is_capitalized = is_capitalized;
    analyzer->word_count++;
    analyzer->unique_words++;
    
    return analyzer->word_count - 1;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) {
        return;
    }
    
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    int in_word = 0;
    int word_start_capital = 0;
    
    for (size_t i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        analyzer->total_chars++;
        
        if (is_word_char(c)) {
            if (!in_word) {
                word_start_capital = isupper((unsigned char)c) ? 1 : 0;
                in_word = 1;
                buf_index = 0;
            }
            
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = tolower((unsigned char)c);
            }
        } else {
            if (in_word && buf_index > 0) {
                buffer[buf_index] = '\0';
                add_word(analyzer, buffer, word_start_capital);
                analyzer->total_words++;
                in_word = 0;
                buf_index = 0;
            }
        }
    }
    
    if (in_word && buf_index > 0) {
        buffer[buf_index] = '\0';
        add_word(analyzer, buffer, word_start_capital);
        analyzer->total_words++;
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total characters: %d\n", analyzer->total_chars);
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("\nWord frequency (top 10):\n");
    
    for (int i = 0; i < analyzer->word_count && i < 10; i++) {
        int max_index = i;
        for (int j = i + 1; j < analyzer->word_count; j++) {
            if (analyzer->words[j].count > analyzer->words[max_index].count) {
                max_index = j;
            }
        }
        
        if (max_index != i) {
            struct WordStats temp = analyzer->words[i];
            analyzer->words[i] = analyzer->words[max_index];
            analyzer->words[max_index] = temp;
        }
        
        printf("%s: %d occurrences", analyzer->words[i].word, analyzer->words[i].count);
        if (analyzer->words[i].is_capitalized) {
            printf(" (capitalized)");
        }
        printf("\n");
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_buffer[4096];
    printf("Enter text to analyze (max 4095 characters): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin)