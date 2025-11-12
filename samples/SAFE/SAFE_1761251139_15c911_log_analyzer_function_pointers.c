//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    printf("Processed line %d: %s", entry->line_number, entry->line);
    if (entry->line_number % 10 == 0) {
        printf("--- Processed %d lines so far ---\n", count);
    }
}

void find_errors(LogEntry* entry) {
    static int error_count = 0;
    char* pos = entry->line;
    
    while (*pos) {
        if (strncmp(pos, "ERROR", 5) == 0) {
            error_count++;
            printf("ERROR found at line %d: %s", entry->line_number, entry->line);
            break;
        }
        pos++;
    }
    
    if (entry->line_number == -1) {
        printf("Total errors found: %d\n", error_count);
    }
}

void analyze_timestamps(LogEntry* entry) {
    static int timestamp_count = 0;
    char* pos = entry->line;
    
    while (*pos && *pos != '\n') {
        if (isdigit((unsigned char)*pos) && isdigit((unsigned char)*(pos+1)) && 
            *(pos+2) == ':' && isdigit((unsigned char)*(pos+3)) && 
            isdigit((unsigned char)*(pos+4))) {
            timestamp_count++;
            printf("Timestamp found at line %d: ", entry->line_number);
            for (int i = 0; i < 5 && *pos && *pos != '\n'; i++) {
                putchar(*pos++);
            }
            printf("\n");
            break;
        }
        pos++;
    }
    
    if (entry->line_number == -1) {
        printf("Total timestamps found: %d\n", timestamp_count);
    }
}

void process_log_file(const char* filename, LogProcessor processor) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return;
    }
    
    LogEntry entry;
    entry.line_number = 0;
    
    while (fgets(entry.line, MAX_LINE_LENGTH, file)) {
        entry.line_number++;
        if (entry.line_number > MAX_LINES) {
            fprintf(stderr, "Warning: Reached maximum line limit\n");
            break;
        }
        
        size_t len = strlen(entry.line);
        if (len > 0 && entry.line[len-1] != '\n') {
            fprintf(stderr, "Warning: Line %d too long, truncating\n", entry.line_number);
            while (fgetc(file) != '\n' && !feof(file));
        }
        
        processor(&entry);
    }
    
    fclose(file);
    
    LogEntry final_entry;
    final_entry.line_number = -1;
    strcpy(final_entry.line, "");
    processor(&final_entry);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    const char* filename = argv[1];
    
    printf("=== Line Counter Analysis ===\n");
    process_log_file(filename, count_lines);
    
    printf("\n=== Error Detection Analysis ===\n");
    process_log_file(filename, find_errors);
    
    printf("\n=== Timestamp Analysis ===\n");
    process_log_file(filename, analyze_timestamps);
    
    return 0;
}