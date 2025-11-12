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
    struct WordStats *words;
    int word_count;
    int capacity;
    char text[1024];
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->words = malloc(sizeof(struct WordStats) * 50);
    if (analyzer->words == NULL) {
        exit(1);
    }
    analyzer->word_count = 0;
    analyzer->capacity = 50;
    analyzer->text[0] = '\0';
}

void free_analyzer(struct TextAnalyzer *analyzer) {
    if (analyzer->words != NULL) {
        free(analyzer->words);
        analyzer->words = NULL;
    }
}

int is_vowel(char c) {
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

int is_consonant(char c) {
    c = tolower(c);
    return isalpha(c) && !is_vowel(c);
}

void analyze_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= analyzer->capacity) {
        int new_capacity = analyzer->capacity * 2;
        struct WordStats *new_words = realloc(analyzer->words, sizeof(struct WordStats) * new_capacity);
        if (new_words == NULL) {
            return;
        }
        analyzer->words = new_words;
        analyzer->capacity = new_capacity;
    }
    
    struct WordStats *current = &analyzer->words[analyzer->word_count];
    strncpy(current->word, word, 63);
    current->word[63] = '\0';
    
    current->length = 0;
    current->vowel_count = 0;
    current->consonant_count = 0;
    
    for (int i = 0; word[i] != '\0' && i < 63; i++) {
        current->length++;
        if (is_vowel(word[i])) {
            current->vowel_count++;
        } else if (is_consonant(word[i])) {
            current->consonant_count++;
        }
    }
    
    analyzer->word_count++;
}

void process_text(struct TextAnalyzer *analyzer) {
    char buffer[1024];
    strncpy(buffer, analyzer->text, 1023);
    buffer[1023] = '\0';
    
    char *token = strtok(buffer, " \t\n\r.,;:!?");
    while (token != NULL) {
        if (strlen(token) > 0) {
            analyze_word(analyzer, token);
        }
        token = strtok(NULL, " \t\n\r.,;:!?");
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->word_count);
    printf("\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *word = &analyzer->words[i];
        printf("Word: %-15s Length: %2d  Vowels: %d  Consonants: %d\n",
               word->word, word->length, word->vowel_count, word->consonant_count);
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    printf("Enter text to analyze (max 1023 characters):\n");
    if (fgets(analyzer.text, sizeof(analyzer.text), stdin) == NULL) {
        printf("Error reading input\n");
        free_analyzer(&analyzer);
        return 1;
    }
    
    size_t len = strlen(analyzer.text);
    if (len > 0 && analyzer.text[len-1] == '\n') {
        analyzer.text[len-1] = '\0';
    }
    
    if (strlen(analyzer.text) == 0) {
        printf("No text entered\n");
        free_analyzer(&analyzer);
        return 1;
    }
    
    process_text(&analyzer);
    print_analysis(&analyzer);
    
    free_analyzer(&analyzer);
    return 0;
}