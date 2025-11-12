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
    printf("Processed %d lines\n", count);
}

void find_errors(LogEntry* entry) {
    if (strstr(entry->line, "ERROR") != NULL) {
        printf("Line %d: %s", entry->line_number, entry->line);
    }
}

void find_warnings(LogEntry* entry) {
    if (strstr(entry->line, "WARNING") != NULL) {
        printf("Line %d: %s", entry->line_number, entry->line);
    }
}

void print_all(LogEntry* entry) {
    printf("Line %d: %s", entry->line_number, entry->line);
}

int validate_line(const char* line) {
    if (line == NULL) return 0;
    size_t len = strlen(line);
    if (len == 0 || len >= MAX_LINE_LENGTH) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(line[i]) && line[i] != '\n' && line[i] != '\t') {
            return 0;
        }
    }
    return 1;
}

void process_log_file(FILE* file, LogProcessor processor) {
    if (file == NULL || processor == NULL) return;
    
    LogEntry entries[MAX_LINES];
    int line_count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (line_count < MAX_LINES && fgets(buffer, sizeof(buffer), file) != NULL) {
        if (!validate_line(buffer)) {
            fprintf(stderr, "Invalid line detected, skipping\n");
            continue;
        }
        
        LogEntry* entry = &entries[line_count];
        strncpy(entry->line, buffer, sizeof(entry->line) - 1);
        entry->line[sizeof(entry->line) - 1] = '\0';
        entry->line_number = line_count + 1;
        
        processor(entry);
        line_count++;
    }
    
    if (!feof(file) && line_count >= MAX_LINES) {
        fprintf(stderr, "Warning: Maximum line count reached\n");
    }
}

int main() {
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
        "Print all"
    };
    
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("Available log processors:\n");
    for (int i = 0; i < num_processors; i++) {
        printf("%d. %s\n", i + 1, processor_names[i]);
    }
    
    printf("Select processor (1-%d): ", num_processors);
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (choice < 1 || choice > num_processors) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    printf("Enter log data (empty line to finish):\n");
    
    FILE* temp_file = tmpfile();
    if (temp_file == NULL) {
        fprintf(stderr, "Failed to create temporary file\n");
        return 1;
    }
    
    char input_line[MAX_LINE_LENGTH];
    while (fgets(input_line, sizeof(input_line), stdin) != NULL) {
        if (strcmp(input_line, "\n") == 0) {
            break;
        }
        if (!validate_line(input_line)) {
            fprintf(stderr, "Invalid input line, skipping\n");
            continue;
        }
        fputs(input_line, temp_file);
    }
    
    rewind(temp_file);
    
    LogProcessor selected_processor = processors[choice - 1];
    printf("\nProcessing log with: %s\n", processor_names[choice - 1]);
    process_log_file(temp_file, selected_processor);
    
    fclose(temp_file);
    return 0;
}