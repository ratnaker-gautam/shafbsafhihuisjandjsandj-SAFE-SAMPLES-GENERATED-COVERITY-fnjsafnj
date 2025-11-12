//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct WordStats {
    char word[64];
    int length;
    int vowel_count;
    int consonant_count;
};

struct TextAnalyzer {
    struct WordStats words[1000];
    int word_count;
    int total_chars;
    int total_vowels;
    int total_consonants;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_chars = 0;
    analyzer->total_vowels = 0;
    analyzer->total_consonants = 0;
}

int is_vowel(char c) {
    c = tolower(c);
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

int is_consonant(char c) {
    c = tolower(c);
    return (c >= 'a' && c <= 'z' && !is_vowel(c));
}

int process_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= 1000) return 0;
    
    int len = 0;
    while (word[len] != '\0' && len < 63) len++;
    if (len == 0) return 0;
    
    struct WordStats *ws = &analyzer->words[analyzer->word_count];
    
    for (int i = 0; i < len && i < 63; i++) {
        ws->word[i] = word[i];
    }
    ws->word[len] = '\0';
    ws->length = len;
    
    ws->vowel_count = 0;
    ws->consonant_count = 0;
    
    for (int i = 0; i < len; i++) {
        if (is_vowel(ws->word[i])) {
            ws->vowel_count++;
            analyzer->total_vowels++;
        } else if (is_consonant(ws->word[i])) {
            ws->consonant_count++;
            analyzer->total_consonants++;
        }
    }
    
    analyzer->total_chars += len;
    analyzer->word_count++;
    return 1;
}

void analyze_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;
    
    char buffer[1024];
    int buf_idx = 0;
    
    for (int i = 0; text[i] != '\0' && i < 1023; i++) {
        if (isspace(text[i])) {
            if (buf_idx > 0) {
                buffer[buf_idx] = '\0';
                process_word(analyzer, buffer);
                buf_idx = 0;
            }
        } else {
            buffer[buf_idx++] = text[i];
            if (buf_idx >= 1023) {
                buffer[buf_idx] = '\0';
                process_word(analyzer, buffer);
                buf_idx = 0;
            }
        }
    }
    
    if (buf_idx > 0) {
        buffer[buf_idx] = '\0';
        process_word(analyzer, buffer);
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->word_count);
    printf("Total characters: %d\n", analyzer->total_chars);
    printf("Total vowels: %d\n", analyzer->total_vowels);
    printf("Total consonants: %d\n", analyzer->total_consonants);
    printf("\nWord Details:\n");
    
    for (int i = 0; i < analyzer->word_count && i < 20; i++) {
        const struct WordStats *ws = &analyzer->words[i];
        printf("Word: %-15s Length: %2d Vowels: %2d Consonants: %2d\n",
               ws->word, ws->length, ws->vowel_count, ws->consonant_count);
    }
    
    if (analyzer->word_count > 20) {
        printf("... and %d more words\n", analyzer->word_count - 20);
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_text[2048];
    printf("Enter text to analyze (max 2047 characters):\n");
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len-1] == '\n') {
        input_text[len-1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    analyze_text(&analyzer, input_text);
    print_analysis(&analyzer);
    
    return 0;
}