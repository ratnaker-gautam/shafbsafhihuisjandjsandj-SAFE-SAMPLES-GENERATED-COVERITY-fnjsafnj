//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef char* (*string_processor)(char*);

char* to_uppercase(char* str) {
    if (str == NULL) return NULL;
    for (char* p = str; *p; p++) {
        *p = toupper((unsigned char)*p);
    }
    return str;
}

char* to_lowercase(char* str) {
    if (str == NULL) return NULL;
    for (char* p = str; *p; p++) {
        *p = tolower((unsigned char)*p);
    }
    return str;
}

char* reverse_string(char* str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
    return str;
}

char* remove_whitespace(char* str) {
    if (str == NULL) return NULL;
    char* dst = str;
    char* src = str;
    while (*src) {
        if (!isspace((unsigned char)*src)) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
    return str;
}

void process_string_array(char** strings, size_t count, string_processor processor) {
    if (strings == NULL || processor == NULL) return;
    for (size_t i = 0; i < count; i++) {
        if (strings[i] != NULL) {
            processor(strings[i]);
        }
    }
}

int main() {
    char input_strings[5][256] = {
        "Hello World",
        "Function Pointers",
        "C Programming",
        "  Spaces  Everywhere  ",
        "MiXeD CaSe"
    };
    
    char* string_ptrs[5];
    for (int i = 0; i < 5; i++) {
        string_ptrs[i] = input_strings[i];
    }
    
    string_processor processors[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    
    const char* processor_names[] = {
        "Uppercase",
        "Lowercase",
        "Reversed",
        "No Whitespace"
    };
    
    for (int proc_idx = 0; proc_idx < 4; proc_idx++) {
        printf("Processing with %s:\n", processor_names[proc_idx]);
        
        char working_strings[5][256];
        for (int i = 0; i < 5; i++) {
            strncpy(working_strings[i], input_strings[i], 255);
            working_strings[i][255] = '\0';
            string_ptrs[i] = working_strings[i];
        }
        
        process_string_array(string_ptrs, 5, processors[proc_idx]);
        
        for (int i = 0; i < 5; i++) {
            printf("  \"%s\"\n", string_ptrs[i]);
        }
        printf("\n");
    }
    
    return 0;
}