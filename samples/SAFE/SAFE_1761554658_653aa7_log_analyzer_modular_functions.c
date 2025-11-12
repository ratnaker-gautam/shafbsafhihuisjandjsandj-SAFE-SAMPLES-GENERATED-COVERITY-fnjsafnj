//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
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
    
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int is_valid_level(const char* level) {
    if (level == NULL) return 0;
    return (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || 
            strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0);
}

void analyze_logs(struct LogEntry logs[], int count) {
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) info_count++;
        else if (strcmp(logs[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(logs[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(logs[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("INFO entries: %d\n", info_count);
    printf("WARN entries: %d\n", warn_count);
    printf("ERROR entries: %d\n", error_count);
    printf("DEBUG entries: %d\n", debug_count);
    printf("Total entries: %d\n", count);
}

void display_logs_by_level(struct LogEntry logs[], int count, const char* level) {
    if (!is_valid_level(level)) {
        printf("Invalid log level: %s\n", level);
        return;
    }
    
    printf("\n%s entries:\n", level);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, level) == 0) {
            printf("%s %s %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
            found = 1;
        }
    }
    if (!found) {
        printf("No %s entries found.\n", level);
    }
}

int read_log_file(const char* filename, struct LogEntry logs[]) {
    if (filename == NULL || logs == NULL) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < MAX_ENTRIES) {
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) > 0) {
            struct LogEntry entry;
            if (parse_log_line(line, &entry) && is_valid_level(entry.level)) {
                logs[count] = entry;
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    char filename[256];
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    filename[strcspn(filename, "\n")] = 0;
    
    if (strlen(filename) == 0) {
        printf("Error: Empty filename.\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, logs);
    if (entry_count == 0) {
        printf("No valid log entries found or file error.\n");
        return 1;
    }
    
    analyze_logs(logs, entry_count);
    
    char level[16];
    printf("\nEnter log level to filter (INFO/WARN/ERROR/DEBUG): ");
    if (fgets(level, sizeof(level), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    level[strcspn(level, "\n")] = 0;
    
    display_logs_by_level(logs, entry_count, level);
    
    return 0;
}