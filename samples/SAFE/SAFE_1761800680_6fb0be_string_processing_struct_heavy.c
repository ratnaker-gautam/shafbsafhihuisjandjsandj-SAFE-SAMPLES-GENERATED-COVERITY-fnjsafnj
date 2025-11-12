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
    analyzer->capacity = 50;
    analyzer->word_count = 0;
    analyzer->text[0] = '\0';
}

void cleanup_analyzer(struct TextAnalyzer *analyzer) {
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
        if (new_words == NULL) return;
        analyzer->words = new_words;
        analyzer->capacity = new_capacity;
    }
    
    struct WordStats *current = &analyzer->words[analyzer->word_count];
    size_t word_len = strlen(word);
    if (word_len >= sizeof(current->word)) {
        word_len = sizeof(current->word) - 1;
    }
    
    strncpy(current->word, word, word_len);
    current->word[word_len] = '\0';
    current->length = (int)word_len;
    
    current->vowel_count = 0;
    current->consonant_count = 0;
    
    for (int i = 0; i < current->length; i++) {
        if (is_vowel(current->word[i])) {
            current->vowel_count++;
        } else if (is_consonant(current->word[i])) {
            current->consonant_count++;
        }
    }
    
    analyzer->word_count++;
}

void process_text(struct TextAnalyzer *analyzer, const char *input) {
    if (input == NULL) return;
    
    size_t input_len = strlen(input);
    if (input_len >= sizeof(analyzer->text)) {
        input_len = sizeof(analyzer->text) - 1;
    }
    
    strncpy(analyzer->text, input, input_len);
    analyzer->text[input_len] = '\0';
    
    analyzer->word_count = 0;
    
    char buffer[1024];
    strncpy(buffer, analyzer->text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    char *token = strtok(buffer, " \t\n\r.,;:!?");
    while (token != NULL && analyzer->word_count < analyzer->capacity) {
        if (strlen(token) > 0) {
            analyze_word(analyzer, token);
        }
        token = strtok(NULL, " \t\n\r.,;:!?");
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text analysis results:\n");
    printf("Input text: %s\n", analyzer->text);
    printf("Total words: %d\n", analyzer->word_count);
    printf("\nWord breakdown:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *word = &analyzer->words[i];
        printf("Word: %-15s Length: %2d  Vowels: %d  Consonants: %d\n",
               word->word, word->length, word->vowel_count, word->consonant_count);
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input[1024];
    printf("Enter text to analyze: ");
    
    if (fgets(input, sizeof(input), stdin) != NULL) {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) > 0) {
            process_text(&analyzer, input);
            print_analysis(&analyzer);
        } else {
            printf("No input provided.\n");
        }
    } else {
        printf("Error reading input.\n");
    }
    
    cleanup_analyzer(&analyzer);
    return 0;
}