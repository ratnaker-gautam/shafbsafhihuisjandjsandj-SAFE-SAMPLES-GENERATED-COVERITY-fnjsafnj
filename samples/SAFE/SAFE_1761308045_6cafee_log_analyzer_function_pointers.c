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

typedef void (*LogProcessor)(LogEntry*, int);

void count_lines(LogEntry* entries, int count) {
    printf("Total log entries: %d\n", count);
}

void find_errors(LogEntry* entries, int count) {
    int error_count = 0;
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].line, "ERROR") != NULL) {
            printf("Line %d: %s", entries[i].line_number, entries[i].line);
            error_count++;
        }
    }
    printf("Total errors found: %d\n", error_count);
}

void find_warnings(LogEntry* entries, int count) {
    int warning_count = 0;
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].line, "WARNING") != NULL) {
            printf("Line %d: %s", entries[i].line_number, entries[i].line);
            warning_count++;
        }
    }
    printf("Total warnings found: %d\n", warning_count);
}

void print_all(LogEntry* entries, int count) {
    for (int i = 0; i < count; i++) {
        printf("Line %d: %s", entries[i].line_number, entries[i].line);
    }
}

int read_log_file(const char* filename, LogEntry* entries) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }
    
    int line_count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (line_count < MAX_LINES && fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strlen(buffer) >= MAX_LINE_LENGTH - 1) {
            fclose(file);
            return -1;
        }
        
        strncpy(entries[line_count].line, buffer, MAX_LINE_LENGTH - 1);
        entries[line_count].line[MAX_LINE_LENGTH - 1] = '\0';
        entries[line_count].line_number = line_count + 1;
        line_count++;
    }
    
    fclose(file);
    return line_count;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    LogEntry entries[MAX_LINES];
    int line_count = read_log_file(argv[1], entries);
    
    if (line_count <= 0) {
        fprintf(stderr, "Error: Could not read log file or file is empty\n");
        return 1;
    }
    
    LogProcessor processors[] = {
        count_lines,
        find_errors,
        find_warnings,
        print_all
    };
    
    const char* processor_names[] = {
        "Count lines",
        "Find errors",
        "Find warnings",
        "Print all entries"
    };
    
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("Log Analysis Menu:\n");
    for (int i = 0; i < num_processors; i++) {
        printf("%d. %s\n", i + 1, processor_names[i]);
    }
    printf("Enter choice (1-%d): ", num_processors);
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char* endptr;
    long choice = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\n' || choice < 1 || choice > num_processors) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    processors[choice - 1](entries, line_count);
    
    return 0;
}