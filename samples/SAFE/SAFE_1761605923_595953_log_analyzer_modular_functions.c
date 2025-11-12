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

void display_recent_logs(struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    printf("\nRecent Log Entries (last 5):\n");
    int start = (count > 5) ? count - 5 : 0;
    for (int i = start; i < count; i++) {
        printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARN, ERROR, DEBUG\n");
    printf("Enter 'quit' on a new line to finish input\n\n");
    
    char line[MAX_LINE_LENGTH];
    
    while (entry_count < MAX_ENTRIES) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "quit") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (validate_log_level(entry.level)) {
                entries[entry_count] = entry;
                entry_count++;
            } else {
                printf("Invalid log level. Supported: INFO, WARN, ERROR, DEBUG\n");
            }
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    analyze_logs(entries, entry_count);
    display_recent_logs(entries, entry_count);
    
    return 0;
}