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
    int line_number;
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_lines(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d lines so far\n", count);
}

void find_errors(LogEntry* entry) {
    static int error_count = 0;
    if (strstr(entry->line, "ERROR") != NULL || strstr(entry->line, "error") != NULL) {
        error_count++;
        printf("Error found at line %d: %s\n", entry->line_number, entry->line);
    }
}

void analyze_timestamps(LogEntry* entry) {
    static int timestamp_count = 0;
    if (strstr(entry->line, ":") != NULL) {
        char* colon = strchr(entry->line, ':');
        if (colon != NULL && colon > entry->line) {
            char* prev_char = colon - 1;
            if (prev_char >= entry->line && isdigit(*prev_char)) {
                timestamp_count++;
            }
        }
    }
    if (entry->line_number % 100 == 0) {
        printf("Found %d potential timestamps so far\n", timestamp_count);
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
    
    LogEntry entry;
    entry.line_number = 0;
    
    while (fgets(entry.line, MAX_LINE_LENGTH, file) != NULL) {
        entry.line_number++;
        size_t len = strlen(entry.line);
        if (len > 0 && entry.line[len - 1] == '\n') {
            entry.line[len - 1] = '\0';
        }
        if (len >= MAX_LINE_LENGTH - 1) {
            fprintf(stderr, "Line %d too long, skipping remainder\n", entry.line_number);
            int c;
            while ((c = fgetc(file)) != '\n' && c != EOF);
        }
        processor(&entry);
    }
    
    if (ferror(file)) {
        fprintf(stderr, "Error reading file\n");
    }
    
    fclose(file);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    printf("=== Log Analysis Results ===\n");
    
    printf("\n1. Line Count Analysis:\n");
    process_log_file(argv[1], count_lines);
    
    printf("\n2. Error Detection:\n");
    process_log_file(argv[1], find_errors);
    
    printf("\n3. Timestamp Analysis:\n");
    process_log_file(argv[1], analyze_timestamps);
    
    printf("\n=== Analysis Complete ===\n");
    
    return 0;
}