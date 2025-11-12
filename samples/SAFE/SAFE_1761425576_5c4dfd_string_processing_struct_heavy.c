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

struct TextProcessor {
    struct WordInfo words[MAX_WORDS];
    int word_count;
    int total_vowels;
    int total_consonants;
};

void initialize_processor(struct TextProcessor *proc) {
    proc->word_count = 0;
    proc->total_vowels = 0;
    proc->total_consonants = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        proc->words[i].word[0] = '\0';
        proc->words[i].length = 0;
        proc->words[i].vowel_count = 0;
        proc->words[i].consonant_count = 0;
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

int process_word(struct TextProcessor *proc, const char *word) {
    if (proc->word_count >= MAX_WORDS) {
        return 0;
    }
    
    size_t len = strlen(word);
    if (len == 0 || len >= MAX_WORD_LEN) {
        return 0;
    }
    
    struct WordInfo *info = &proc->words[proc->word_count];
    strncpy(info->word, word, MAX_WORD_LEN - 1);
    info->word[MAX_WORD_LEN - 1] = '\0';
    info->length = (int)len;
    
    info->vowel_count = 0;
    info->consonant_count = 0;
    
    for (int i = 0; i < info->length; i++) {
        if (is_vowel(info->word[i])) {
            info->vowel_count++;
        } else if (is_consonant(info->word[i])) {
            info->consonant_count++;
        }
    }
    
    proc->total_vowels += info->vowel_count;
    proc->total_consonants += info->consonant_count;
    proc->word_count++;
    
    return 1;
}

void tokenize_and_process(struct TextProcessor *proc, const char *text) {
    if (text == NULL) {
        return;
    }
    
    char buffer[512];
    strncpy(buffer, text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    char *token = strtok(buffer, " \t\n\r.,;:!?");
    while (token != NULL && proc->word_count < MAX_WORDS) {
        if (strlen(token) > 0) {
            process_word(proc, token);
        }
        token = strtok(NULL, " \t\n\r.,;:!?");
    }
}

void print_statistics(const struct TextProcessor *proc) {
    printf("Text Analysis Results:\n");
    printf("Total words processed: %d\n", proc->word_count);
    printf("Total vowels: %d\n", proc->total_vowels);
    printf("Total consonants: %d\n", proc->total_consonants);
    printf("\nWord Details:\n");
    
    for (int i = 0; i < proc->word_count; i++) {
        const struct WordInfo *info = &proc->words[i];
        printf("Word: %-15s Length: %2d  Vowels: %d  Consonants: %d\n",
               info->word, info->length, info->vowel_count, info->consonant_count);
    }
}

int main(void) {
    struct TextProcessor processor;
    initialize_processor(&processor);
    
    char input_text[1024];
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
    
    tokenize_and_process(&processor, input_text);
    
    if (processor.word_count == 0) {
        printf("No valid words found in input.\n");
        return 1;
    }
    
    print_statistics(&processor);
    
    return 0;
}