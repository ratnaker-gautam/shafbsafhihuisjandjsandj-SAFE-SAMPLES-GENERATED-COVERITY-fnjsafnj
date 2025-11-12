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
    char message[MAX_LINE_LENGTH];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries\n", count);
}

void show_errors(LogEntry* entry) {
    if (strstr(entry->level, "ERROR") != NULL) {
        printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    if (strstr(entry->level, "WARN") != NULL) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void process_log_file(FILE* file, LogProcessor processor) {
    if (file == NULL || processor == NULL) return;
    
    char line[MAX_LINE_LENGTH];
    LogEntry entry;
    int line_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && line_count < MAX_ENTRIES) {
        line_count++;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (parse_log_line(line, &entry)) {
            processor(&entry);
        }
    }
}

int main() {
    printf("Log Analyzer\n");
    printf("Available modes:\n");
    printf("1. Count entries\n");
    printf("2. Show errors\n");
    printf("3. Show warnings\n");
    printf("Enter choice (1-3): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    LogProcessor processors[] = {count_entries, show_errors, show_warnings};
    const char* mode_names[] = {"Counting entries", "Showing errors", "Showing warnings"};
    
    if (choice < 1 || choice > 3) {
        printf("Invalid choice\n");
        return 1;
    }
    
    LogProcessor selected_processor = processors[choice - 1];
    printf("Mode: %s\n", mode_names[choice - 1]);
    printf("Enter log data (empty line to finish):\n");
    
    char filename[] = "temp_log.txt";
    FILE* temp_file = fopen(filename, "w");
    if (temp_file == NULL) {
        printf("Error creating temporary file\n");
        return 1;
    }
    
    char input_line[MAX_LINE_LENGTH];
    while (fgets(input_line, sizeof(input_line), stdin) != NULL) {
        size_t len = strlen(input_line);
        if (len > 0 && input_line[len - 1] == '\n') {
            input_line[len - 1] = '\0';
        }
        
        if (strlen(input_line) == 0) {
            break;
        }
        
        if (fprintf(temp_file, "%s\n", input_line) < 0) {
            printf("Error writing to file\n");
            fclose(temp_file);
            remove(filename);
            return 1;
        }
    }
    
    fclose(temp_file);
    
    FILE* log_file = fopen(filename, "r");
    if (log_file == NULL) {
        printf("Error opening log file\n");
        remove(filename);
        return 1;
    }
    
    process_log_file(log_file, selected_processor);
    
    fclose(log_file);
    remove(filename);
    
    return 0;
}