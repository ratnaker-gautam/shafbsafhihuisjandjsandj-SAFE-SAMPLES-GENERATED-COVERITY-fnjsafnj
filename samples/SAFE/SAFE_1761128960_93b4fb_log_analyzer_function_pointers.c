//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LINES 1000

typedef struct {
    char line[MAX_LINE_LENGTH];
    int length;
} LogEntry;

typedef void (*LogProcessor)(LogEntry*, int);

void count_lines(LogEntry* entries, int count) {
    printf("Total lines: %d\n", count);
}

void find_longest_line(LogEntry* entries, int count) {
    int max_length = 0;
    int max_index = -1;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].length > max_length) {
            max_length = entries[i].length;
            max_index = i;
        }
    }
    
    if (max_index != -1) {
        printf("Longest line (%d chars): %s\n", max_length, entries[max_index].line);
    }
}

void count_uppercase(LogEntry* entries, int count) {
    int total_upper = 0;
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < entries[i].length; j++) {
            if (isupper((unsigned char)entries[i].line[j])) {
                total_upper++;
            }
        }
    }
    
    printf("Total uppercase letters: %d\n", total_upper);
}

void process_log(LogEntry* entries, int count, LogProcessor processor) {
    if (entries == NULL || count <= 0 || processor == NULL) {
        return;
    }
    processor(entries, count);
}

int read_log_file(LogEntry* entries, int max_entries) {
    if (entries == NULL || max_entries <= 0) {
        return 0;
    }
    
    int count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    printf("Enter log lines (empty line to finish):\n");
    
    while (count < max_entries) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            break;
        }
        
        if (len >= MAX_LINE_LENGTH) {
            printf("Line too long, skipping.\n");
            continue;
        }
        
        strncpy(entries[count].line, buffer, MAX_LINE_LENGTH - 1);
        entries[count].line[MAX_LINE_LENGTH - 1] = '\0';
        entries[count].length = (int)len;
        count++;
    }
    
    return count;
}

int main(void) {
    LogEntry entries[MAX_LINES];
    LogProcessor processors[] = {
        count_lines,
        find_longest_line,
        count_uppercase
    };
    const char* processor_names[] = {
        "Count lines",
        "Find longest line",
        "Count uppercase letters"
    };
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    int line_count = read_log_file(entries, MAX_LINES);
    
    if (line_count == 0) {
        printf("No log entries to process.\n");
        return 1;
    }
    
    printf("\nAvailable analysis functions:\n");
    for (int i = 0; i < num_processors; i++) {
        printf("%d. %s\n", i + 1, processor_names[i]);
    }
    
    printf("Enter function number (1-%d): ", num_processors);
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (choice < 1 || choice > num_processors) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    printf("\n");
    process_log(entries, line_count, processors[choice - 1]);
    
    return 0;
}