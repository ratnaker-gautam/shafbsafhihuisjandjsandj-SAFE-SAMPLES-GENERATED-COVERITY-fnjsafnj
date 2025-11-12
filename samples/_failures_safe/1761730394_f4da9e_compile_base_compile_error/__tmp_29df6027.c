//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000
#define MAX_LINE_LEN 256

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
    int longest_word_length;
};

struct TextProcessor {
    char input_buffer[MAX_LINE_LEN];
    char output_buffer[MAX_LINE_LEN];
    int buffer_pos;
    int line_count;
};

void init_text_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_chars = 0;
    analyzer->total_words = 0;
    analyzer->unique_words = 0;
    analyzer->longest_word_length = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].word[0] = '\0';
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
    }
}

void init_text_processor(struct TextProcessor *processor) {
    processor->input_buffer[0] = '\0';
    processor->output_buffer[0] = '\0';
    processor->buffer_pos = 0;
    processor->line_count = 0;
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

void add_word(struct TextAnalyzer *analyzer, const char *word, int length, int capitalized) {
    if (length <= 0 || length >= MAX_WORD_LEN) return;
    
    int index = find_word_index(analyzer, word);
    if (index >= 0) {
        analyzer->words[index].count++;
        return;
    }
    
    if (analyzer->word_count >= MAX_WORDS) return;
    
    struct WordStats *ws = &analyzer->words[analyzer->word_count];
    strncpy(ws->word, word, MAX_WORD_LEN - 1);
    ws->word[MAX_WORD_LEN - 1] = '\0';
    ws->count = 1;
    ws->length = length;
    ws->is_capitalized = capitalized;
    
    analyzer->word_count++;
    analyzer->unique_words++;
    
    if (length > analyzer->longest_word_length) {
        analyzer->longest_word_length = length;
    }
}

void process_line(struct TextAnalyzer *analyzer, struct TextProcessor *processor, const char *line) {
    if (!line || strlen(line) >= MAX_LINE_LEN) return;
    
    processor->line_count++;
    strncpy(processor->input_buffer, line, MAX_LINE_LEN - 1);
    processor->input_buffer[MAX_LINE_LEN - 1] = '\0';
    
    int pos = 0;
    int line_len = strlen(processor->input_buffer);
    
    while (pos < line_len) {
        while (pos < line_len && !is_word_char(processor->input_buffer[pos])) {
            pos++;
        }
        
        if (pos >= line_len) break;
        
        int start = pos;
        while (pos < line_len && is_word_char(processor->input_buffer[pos])) {
            pos++;
        }
        
        int word_len = pos - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN];
            strncpy(temp_word, &processor->input_buffer[start], word_len);
            temp_word[word_len] = '\0';
            
            int capitalized = isupper((unsigned char)temp_word[0]) ? 1 : 0;
            for (int i = 0; i < word_len; i++) {
                temp_word[i] = tolower((unsigned char)temp_word[i]);
            }
            
            add_word(analyzer, temp_word, word_len, capitalized);
            analyzer->total_words++;
            analyzer->total_chars += word_len;
        }
    }
}

void generate_report(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Report:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("Total characters in words: %d\n", analyzer->total_chars);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Lines processed: %d\n", analyzer->total_words > 0 ? 1 : 0);
    
    printf("\nWord frequency (top 10):\n");
    for (int i = 0; i < (analyzer->word_count < 10 ? analyzer->word_count : 10); i++) {
        printf("%s: %d occurrences\n", analyzer->words[i].word, analyzer->words[i].count);
    }
}

int main(void) {
    struct TextAnaly