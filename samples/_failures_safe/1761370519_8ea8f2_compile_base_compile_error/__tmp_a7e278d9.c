//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[64];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 64 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int is_valid_level(const char* level) {
    const char* valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) return 1;
    }
    return 0;
}

void analyze_logs(struct LogEntry* entries, int count) {
    int info_count = 0, warning_count = 0, error_count = 0, debug_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("INFO: %d entries\n", info_count);
    printf("WARNING: %d entries\n", warning_count);
    printf("ERROR: %d entries\n", error_count);
    printf("DEBUG: %d entries\n", debug_count);
    printf("Total: %d entries\n", count);
}

void display_entries_by_level(struct LogEntry* entries, int count, const char* level) {
    printf("\nEntries with level %s:\n", level);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found = 1;
        }
    }
    if (!found) printf("No entries found for level %s\n", level);
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) return -1;
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < max_entries) {
        line[strcspn(line, "\n")] = '\0';
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry) && is_valid_level(entry.level)) {
            entries[entry_count] = entry;
            entry_count++;
        }
    }
    
    fclose(file);
    return entry_count;
}

int get_user_choice() {
    printf("\nLog Analyzer Menu:\n");
    printf("1. Show analysis summary\n");
    printf("2. Show INFO entries\n");
    printf("3. Show WARNING entries\n");
    printf("4. Show ERROR entries\n");
    printf("5. Show DEBUG entries\n");
    printf("6. Exit\n");
    printf("Enter your choice (1-6): ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) return 6;
    
    input[strcspn(input, "\n")] = '\0';
    
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isdigit(input[i])) return 6;
    }
    
    int choice = atoi(input);
    if (choice < 1 || choice > 6) return 6;
    
    return choice;
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    const char* filename = "sample.log";
    
    printf("Reading log file: %s\n", filename);
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    
    if (entry_count < 0) {
        printf("Error: Could not open log file '%s'\n", filename);
        printf("Creating sample log file for demonstration...\n");
        
        FILE* sample = fopen(filename, "w");
        if (sample != NULL) {
            fprintf(sample, "2024-01-15T10:30:00 INFO Application started\n");
            fprintf(sample, "2024-01-