//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')

typedef struct {
    char text[MAX_WORD_LEN];
    int length;
} Word;

int safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return (int)len;
}

int parse_words(const char *input, Word *words, int max_words) {
    int word_count = 0;
    int i = 0;
    int input_len = safe_strlen(input, MAX_INPUT_LEN);
    
    while (i < input_len && word_count < max_words) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        int word_start = i;
        int word_len = 0;
        
        while (i < input_len && !IS_WHITESPACE(input[i]) && word_len < MAX_WORD_LEN - 1) {
            word_len++;
            i++;
        }
        
        if (word_len > 0) {
            strncpy(words[word_count].text, &input[word_start], word_len);
            words[word_count].text[word_len] = '\0';
            words[word_count].length = word_len;
            word_count++;
        }
    }
    
    return word_count;
}

void capitalize_words(Word *words, int word_count) {
    for (int i = 0; i < word_count; i++) {
        if (words[i].length > 0 && islower(words[i].text[0])) {
            words[i].text[0] = (char)toupper(words[i].text[0]);
        }
    }
}

void reverse_words(Word *words, int word_count) {
    for (int i = 0; i < word_count / 2; i++) {
        Word temp = words[i];
        words[i] = words[word_count - 1 - i];
        words[word_count - 1 - i] = temp;
    }
}

int build_output(const Word *words, int word_count, char *output, int output_size) {
    int pos = 0;
    
    for (int i = 0; i < word_count; i++) {
        int word_len = safe_strlen(words[i].text, MAX_WORD_LEN);
        
        if (pos + word_len + 1 >= output_size) {
            return -1;
        }
        
        strncpy(&output[pos], words[i].text, word_len);
        pos += word_len;
        
        if (i < word_count - 1) {
            output[pos] = ' ';
            pos++;
        }
    }
    
    output[pos] = '\0';
    return pos;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    Word words[MAX_WORDS];
    char output[MAX_INPUT_LEN];
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int word_count = parse_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        fprintf(stderr, "No words found\n");
        return 1;
    }
    
    capitalize_words(words, word_count);
    reverse_words(words, word_count);
    
    if (build_output(words, word_count, output, sizeof(output)) < 0) {
        fprintf(stderr, "Output buffer too small\n");
        return 1;
    }
    
    printf("Processed: %s\n", output);
    return 0;
}