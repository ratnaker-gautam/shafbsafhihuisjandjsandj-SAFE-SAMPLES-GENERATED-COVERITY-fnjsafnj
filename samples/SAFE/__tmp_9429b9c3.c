//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 100
#define MAX_TEXT_LEN 1024

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_chars;
    int total_words;
    int unique_words;
};

void init_processor(struct TextProcessor *proc) {
    proc->word_count = 0;
    proc->total_chars = 0;
    proc->total_words = 0;
    proc->unique_words = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        proc->words[i].word[0] = '\0';
        proc->words[i].count = 0;
        proc->words[i].length = 0;
        proc->words[i].is_capitalized = 0;
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct TextProcessor *proc, const char *word) {
    for (int i = 0; i < proc->word_count; i++) {
        if (strcmp(proc->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextProcessor *proc, const char *word, int capitalized) {
    if (proc->word_count >= MAX_WORDS) {
        return 0;
    }
    
    int len = strlen(word);
    if (len >= MAX_WORD_LEN) {
        return 0;
    }
    
    strcpy(proc->words[proc->word_count].word, word);
    proc->words[proc->word_count].count = 1;
    proc->words[proc->word_count].length = len;
    proc->words[proc->word_count].is_capitalized = capitalized;
    proc->word_count++;
    proc->unique_words++;
    
    return 1;
}

void process_text(struct TextProcessor *proc, const char *text) {
    char buffer[MAX_TEXT_LEN];
    if (strlen(text) >= MAX_TEXT_LEN - 1) {
        return;
    }
    strcpy(buffer, text);
    
    char *ptr = buffer;
    while (*ptr) {
        while (*ptr && !is_word_char(*ptr)) {
            ptr++;
        }
        
        if (!*ptr) break;
        
        char *word_start = ptr;
        while (*ptr && is_word_char(*ptr)) {
            ptr++;
        }
        
        char temp_word[MAX_WORD_LEN];
        int word_len = ptr - word_start;
        if (word_len >= MAX_WORD_LEN) {
            continue;
        }
        
        strncpy(temp_word, word_start, word_len);
        temp_word[word_len] = '\0';
        
        int capitalized = isupper((unsigned char)temp_word[0]) ? 1 : 0;
        for (int i = 0; temp_word[i]; i++) {
            temp_word[i] = tolower((unsigned char)temp_word[i]);
        }
        
        int index = find_word_index(proc, temp_word);
        if (index >= 0) {
            proc->words[index].count++;
        } else {
            if (!add_word(proc, temp_word, capitalized)) {
                break;
            }
        }
        
        proc->total_words++;
        proc->total_chars += word_len;
        
        if (*ptr) ptr++;
    }
}

void print_stats(struct TextProcessor *proc) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", proc->total_words);
    printf("Total characters in words: %d\n", proc->total_chars);
    printf("Unique words: %d\n", proc->unique_words);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < proc->word_count; i++) {
        printf("%s: %d occurrences", proc->words[i].word, proc->words[i].count);
        if (proc->words[i].is_capitalized) {
            printf(" (originally capitalized)");
        }
        printf("\n");
    }
}

int main() {
    struct TextProcessor processor;
    init_processor(&processor);
    
    char input_text[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters): ", MAX_TEXT_LEN - 1);
    
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
    
    process_text(&processor, input_text);
    print_stats(&processor);
    
    return 0;
}