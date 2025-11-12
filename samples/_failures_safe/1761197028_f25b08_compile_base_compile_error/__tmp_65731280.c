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

struct TextStats {
    struct WordInfo words[MAX_WORDS];
    int word_count;
    int total_vowels;
    int total_consonants;
    int longest_word_length;
    int shortest_word_length;
};

struct TextProcessor {
    struct TextStats stats;
    char input_text[1000];
    char processed_text[1000];
};

void initialize_processor(struct TextProcessor *proc) {
    memset(proc, 0, sizeof(struct TextProcessor));
    proc->stats.shortest_word_length = MAX_WORD_LEN;
}

int is_valid_char(char c) {
    return isalpha(c) || isspace(c);
}

int count_vowels(const char *str) {
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        char c = tolower(str[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int count_consonants(const char *str, int len) {
    int count = 0;
    for (int i = 0; i < len; i++) {
        char c = tolower(str[i]);
        if (isalpha(c) && c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
            count++;
        }
    }
    return count;
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    if (strlen(input) >= 1000) return 0;
    
    for (int i = 0; input[i] != '\0'; i++) {
        if (!is_valid_char(input[i])) {
            return 0;
        }
    }
    return 1;
}

int process_text(struct TextProcessor *proc) {
    if (!validate_input(proc->input_text)) {
        return 0;
    }
    
    char temp[1000];
    strcpy(temp, proc->input_text);
    
    char *token = strtok(temp, " ");
    int word_index = 0;
    
    while (token != NULL && word_index < MAX_WORDS) {
        int len = strlen(token);
        if (len > 0 && len < MAX_WORD_LEN) {
            strcpy(proc->stats.words[word_index].word, token);
            proc->stats.words[word_index].length = len;
            proc->stats.words[word_index].vowel_count = count_vowels(token);
            proc->stats.words[word_index].consonant_count = count_consonants(token, len);
            
            proc->stats.total_vowels += proc->stats.words[word_index].vowel_count;
            proc->stats.total_consonants += proc->stats.words[word_index].consonant_count;
            
            if (len > proc->stats.longest_word_length) {
                proc->stats.longest_word_length = len;
            }
            if (len < proc->stats.shortest_word_length) {
                proc->stats.shortest_word_length = len;
            }
            
            word_index++;
        }
        token = strtok(NULL, " ");
    }
    
    proc->stats.word_count = word_index;
    
    if (proc->stats.word_count > 0) {
        strcpy(proc->processed_text, proc->input_text);
        for (int i = 0; proc->processed_text[i] != '\0'; i++) {
            proc->processed_text[i] = tolower(proc->processed_text[i]);
        }
        return 1;
    }
    
    return 0;
}

void print_stats(const struct TextProcessor *proc) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", proc->stats.word_count);
    printf("Total vowels: %d\n", proc->stats.total_vowels);
    printf("Total consonants: %d\n", proc->stats.total_consonants);
    printf("Longest word length: %d\n", proc->stats.longest_word_length);
    printf("Shortest word length: %d\n", proc->stats.shortest_word_length);
    
    printf("\nWord Details:\n");
    for (int i = 0; i < proc->stats.word_count; i++) {
        printf("Word: %-15s Length: %2d Vowels: %d Consonants: %d\n",
               proc->stats.words[i].word,
               proc->stats.words[i].length,
               proc->stats.words[i].vowel_count,
               proc->stats.words[i].consonant_count);
    }
    
    printf("\nProcessed text (lowercase): %s\n", proc->processed_text);
}

int main() {
    struct TextProcessor processor;
    initialize_processor(&processor);
    
    printf("Enter text to analyze (letters and spaces only, max 999 chars):\n");
    
    if (fgets(processor.input_text, sizeof(processor.input_text), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(processor.input_text);
    if (len > 0 && processor.input_text[len -