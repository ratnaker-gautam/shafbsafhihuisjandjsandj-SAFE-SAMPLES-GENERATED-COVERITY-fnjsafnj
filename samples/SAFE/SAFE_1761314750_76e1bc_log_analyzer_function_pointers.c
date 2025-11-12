//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LINES 1000

typedef struct {
    char line[MAX_LINE_LENGTH];
    int length;
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_lines(LogEntry* entry) {
    static int line_count = 0;
    line_count++;
    printf("Processed %d lines\n", line_count);
}

void find_longest_line(LogEntry* entry) {
    static int max_length = 0;
    if (entry->length > max_length) {
        max_length = entry->length;
        printf("New longest line: %d characters\n", max_length);
    }
}

void count_uppercase(LogEntry* entry) {
    static int uppercase_count = 0;
    for (int i = 0; i < entry->length; i++) {
        if (isupper(entry->line[i])) {
            uppercase_count++;
        }
    }
    printf("Total uppercase letters: %d\n", uppercase_count);
}

int read_log_file(const char* filename, LogEntry entries[], int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return -1;
    }

    int count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (count < max_entries && fgets(buffer, sizeof(buffer), file)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        if (len >= sizeof(entries[count].line)) {
            fclose(file);
            return -1;
        }
        
        strncpy(entries[count].line, buffer, sizeof(entries[count].line) - 1);
        entries[count].line[sizeof(entries[count].line) - 1] = '\0';
        entries[count].length = (int)len;
        count++;
    }
    
    fclose(file);
    return count;
}

void process_log_entries(LogEntry entries[], int count, LogProcessor processors[], int processor_count) {
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < processor_count; j++) {
            processors[j](&entries[i]);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    LogEntry entries[MAX_LINES];
    int entry_count = read_log_file(argv[1], entries, MAX_LINES);
    
    if (entry_count <= 0) {
        fprintf(stderr, "Error reading log file or file empty\n");
        return 1;
    }

    LogProcessor processors[] = {count_lines, find_longest_line, count_uppercase};
    int processor_count = sizeof(processors) / sizeof(processors[0]);
    
    printf("Processing %d log entries...\n", entry_count);
    process_log_entries(entries, entry_count, processors, processor_count);
    
    printf("Log analysis complete.\n");
    return 0;
}