//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries\n", count);
}

void print_errors(LogEntry* entry) {
    if (strcmp(entry->level, "ERROR") == 0) {
        printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
    }
}

void print_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARNING") == 0) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 31) return 0;
    if (strlen(level) >= 15) return 0;
    if (strlen(message) >= 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void process_log_file(const char* filename, LogProcessor processor) {
    if (filename == NULL || processor == NULL) {
        printf("Invalid parameters\n");
        return;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open file: %s\n", filename);
        return;
    }
    
    char line[MAX_LINE_LENGTH];
    LogEntry entry;
    int line_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL) {
        line_count++;
        if (line_count > MAX_ENTRIES) {
            printf("Reached maximum entries limit\n");
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (parse_log_line(line, &entry)) {
            processor(&entry);
        } else {
            printf("Failed to parse line %d\n", line_count);
        }
    }
    
    if (fclose(file) != 0) {
        printf("Error closing file\n");
    }
}

int main() {
    char filename[256];
    
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Filename cannot be empty\n");
        return 1;
    }
    
    printf("\n1. Count all entries\n");
    printf("2. Show errors only\n");
    printf("3. Show warnings only\n");
    printf("Choose option: ");
    
    char choice_str[16];
    if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    int choice = atoi(choice_str);
    LogProcessor processor = NULL;
    
    switch (choice) {
        case 1:
            processor = count_entries;
            break;
        case 2:
            processor = print_errors;
            break;
        case 3:
            processor = print_warnings;
            break;
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    process_log_file(filename, processor);
    
    return 0;
}