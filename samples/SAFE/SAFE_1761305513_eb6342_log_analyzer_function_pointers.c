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
    printf("Processed line %d: %s\n", line_count, entry->line);
}

void find_errors(LogEntry* entry) {
    if (strstr(entry->line, "ERROR") != NULL) {
        printf("Found ERROR: %s\n", entry->line);
    }
}

void analyze_length(LogEntry* entry) {
    if (entry->length > 80) {
        printf("Long line (%d chars): %.80s...\n", entry->length, entry->line);
    }
}

void process_log_file(const char* filename, LogProcessor processor) {
    if (filename == NULL || processor == NULL) {
        fprintf(stderr, "Invalid input parameters\n");
        return;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return;
    }

    LogEntry entries[MAX_LINES];
    int entry_count = 0;
    char buffer[MAX_LINE_LENGTH];

    while (entry_count < MAX_LINES && fgets(buffer, sizeof(buffer), file) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }

        if (len >= MAX_LINE_LENGTH - 1) {
            fprintf(stderr, "Line too long, skipping\n");
            continue;
        }

        if (len > 0) {
            strncpy(entries[entry_count].line, buffer, MAX_LINE_LENGTH - 1);
            entries[entry_count].line[MAX_LINE_LENGTH - 1] = '\0';
            entries[entry_count].length = (int)len;
            entry_count++;
        }
    }

    if (ferror(file)) {
        fprintf(stderr, "Error reading file\n");
        fclose(file);
        return;
    }

    fclose(file);

    printf("Processing %d log entries...\n", entry_count);
    for (int i = 0; i < entry_count; i++) {
        processor(&entries[i]);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    LogProcessor processors[] = {count_lines, find_errors, analyze_length};
    const char* names[] = {"Line Counter", "Error Finder", "Length Analyzer"};
    int num_processors = sizeof(processors) / sizeof(processors[0]);

    for (int i = 0; i < num_processors; i++) {
        printf("\n=== %s ===\n", names[i]);
        process_log_file(argv[1], processors[i]);
    }

    return 0;
}