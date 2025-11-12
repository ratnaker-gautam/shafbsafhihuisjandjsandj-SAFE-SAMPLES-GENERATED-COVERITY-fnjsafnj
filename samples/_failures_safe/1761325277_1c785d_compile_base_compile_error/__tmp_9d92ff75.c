//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

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

int validate_log_level(const char* level) {
    if (level == NULL) return 0;
    return (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || 
            strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0);
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("INFO entries: %d\n", info_count);
    printf("WARN entries: %d\n", warn_count);
    printf("ERROR entries: %d\n", error_count);
    printf("DEBUG entries: %d\n", debug_count);
    printf("Total entries: %d\n", count);
}

void display_recent_logs(struct LogEntry* entries, int count, int num_to_show) {
    if (entries == NULL || count <= 0 || num_to_show <= 0) return;
    
    if (num_to_show > count) num_to_show = count;
    
    printf("\nRecent %d log entries:\n", num_to_show);
    printf("%-20s %-8s %s\n", "Timestamp", "Level", "Message");
    printf("------------------------------------------------------------\n");
    
    int start = count - num_to_show;
    if (start < 0) start = 0;
    
    for (int i = start; i < count; i++) {
        printf("%-20s %-8s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log lines (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARN, ERROR, DEBUG\n");
    printf("Enter 'END' on a new line to finish input\n");
    printf("Enter 'SHOW' to display recent logs\n");
    printf("Enter 'ANALYZE' to generate statistics\n\n");
    
    char input_line[MAX_LINE_LENGTH];
    
    while (entry_count < MAX_ENTRIES) {
        printf("> ");
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) break;
        
        size_t len = strlen(input_line);
        if (len > 0 && input_line[len - 1] == '\n') {
            input_line[len - 1] = '\0';
        }
        
        if (strcmp(input_line, "END") == 0) {
            break;
        } else if (strcmp(input_line, "ANALYZE") == 0) {
            analyze_logs(entries, entry_count);
        } else if (strcmp(input_line, "SHOW") == 0) {
            display_recent_logs(entries, entry_count, 5);
        } else if (strlen(input_line) > 0) {
            struct LogEntry new_entry;
            if (parse_log_line(input_line, &new_entry)) {
                if (validate_log_level(new_entry.level)) {
                    entries[entry_count] = new_entry;
                    entry_count++;
                    printf("Added log entry: %s %s %s\n", new_entry.timestamp, new_entry.level, new_entry.message);
                } else {
                    printf("Error: Invalid log level. Supported: INFO, WARN, ERROR, DEBUG\n");
                }
            } else {
                printf("Error: Invalid log format. Use: TIMESTAMP LEVEL M