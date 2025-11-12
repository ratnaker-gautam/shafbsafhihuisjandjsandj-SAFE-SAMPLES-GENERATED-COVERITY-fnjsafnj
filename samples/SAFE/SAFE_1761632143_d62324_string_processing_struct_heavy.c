//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50

struct WordInfo {
    char word[MAX_WORD_LEN];
    int length;
    int vowel_count;
    int consonant_count;
};

struct TextAnalyzer {
    struct WordInfo words[MAX_WORDS];
    int word_count;
    int total_vowels;
    int total_consonants;
    int longest_word_length;
    int shortest_word_length;
};

void initialize_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_vowels = 0;
    analyzer->total_consonants = 0;
    analyzer->longest_word_length = 0;
    analyzer->shortest_word_length = MAX_WORD_LEN;
}

int is_vowel(char c) {
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

int is_consonant(char c) {
    c = tolower(c);
    return isalpha(c) && !is_vowel(c);
}

int process_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= MAX_WORDS) {
        return 0;
    }
    
    size_t len = strlen(word);
    if (len == 0 || len >= MAX_WORD_LEN) {
        return 0;
    }
    
    struct WordInfo *current = &analyzer->words[analyzer->word_count];
    
    if (strlen(word) >= MAX_WORD_LEN) {
        return 0;
    }
    strncpy(current->word, word, MAX_WORD_LEN - 1);
    current->word[MAX_WORD_LEN - 1] = '\0';
    
    current->length = (int)len;
    current->vowel_count = 0;
    current->consonant_count = 0;
    
    for (int i = 0; i < current->length; i++) {
        if (is_vowel(current->word[i])) {
            current->vowel_count++;
        } else if (is_consonant(current->word[i])) {
            current->consonant_count++;
        }
    }
    
    analyzer->total_vowels += current->vowel_count;
    analyzer->total_consonants += current->consonant_count;
    
    if (current->length > analyzer->longest_word_length) {
        analyzer->longest_word_length = current->length;
    }
    if (current->length < analyzer->shortest_word_length) {
        analyzer->shortest_word_length = current->length;
    }
    
    analyzer->word_count++;
    return 1;
}

void tokenize_and_process(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) {
        return;
    }
    
    char buffer[512];
    if (strlen(text) >= sizeof(buffer)) {
        return;
    }
    strncpy(buffer, text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    char *token = strtok(buffer, " \t\n\r.,;:!?");
    while (token != NULL && analyzer->word_count < MAX_WORDS) {
        if (strlen(token) > 0) {
            process_word(analyzer, token);
        }
        token = strtok(NULL, " \t\n\r.,;:!?");
    }
}

void display_analysis(const struct TextAnalyzer *analyzer) {
    if (analyzer->word_count == 0) {
        printf("No words to analyze.\n");
        return;
    }
    
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->word_count);
    printf("Total vowels: %d\n", analyzer->total_vowels);
    printf("Total consonants: %d\n", analyzer->total_consonants);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    
    printf("\nWord Details:\n");
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordInfo *word = &analyzer->words[i];
        printf("Word: %-15s Length: %2d Vowels: %d Consonants: %d\n",
               word->word, word->length, word->vowel_count, word->consonant_count);
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    char input_text[1024];
    
    initialize_analyzer(&analyzer);
    
    printf("Enter text to analyze (max 1023 characters): ");
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    tokenize_and_process(&analyzer, input_text);
    display_analysis(&analyzer);
    
    return 0;
}