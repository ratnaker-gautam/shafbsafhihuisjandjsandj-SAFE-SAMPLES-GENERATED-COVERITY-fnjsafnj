//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp, line);
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    if (strlen(timestamp) >= 19 || strlen(level) >= 9 || strlen(message) >= MAX_LINE_LENGTH - 30) return 0;
    
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

void display_recent_entries(struct LogEntry* entries, int count, int num_to_show) {
    if (entries == NULL || count <= 0 || num_to_show <= 0) return;
    
    if (num_to_show > count) num_to_show = count;
    
    printf("\nRecent %d log entries:\n", num_to_show);
    for (int i = count - num_to_show; i < count; i++) {
        printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARN, ERROR, DEBUG\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry new_entry;
        if (parse_log_line(line, &new_entry)) {
            if (is_valid_level(new_entry.level)) {
                entries[entry_count] = new_entry;
                entry_count++;
            } else {
                printf("Invalid log level. Supported: INFO, WARN, ERROR, DEBUG\n");
            }
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count);
    
    int show_recent;
    printf("\nHow many recent entries to display? (0 for none): ");
    if (scanf("%d", &show_recent) == 1) {
        if (show_recent > 0) {
            display_recent_entries(entries, entry_count, show_recent);
        }
    }
    
    return 0;
}