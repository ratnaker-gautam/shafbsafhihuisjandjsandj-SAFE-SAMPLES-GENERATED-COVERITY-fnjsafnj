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
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void analyze_log_levels(struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int info_count = 0, warning_count = 0, error_count = 0, other_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else {
            other_count++;
        }
    }
    
    printf("Log Level Analysis:\n");
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    printf("OTHER: %d\n", other_count);
}

void find_most_frequent_message(struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int max_count = 0;
    char most_frequent[256] = "";
    
    for (int i = 0; i < count; i++) {
        int current_count = 0;
        for (int j = 0; j < count; j++) {
            if (strcmp(entries[i].message, entries[j].message) == 0) {
                current_count++;
            }
        }
        if (current_count > max_count) {
            max_count = current_count;
            strcpy(most_frequent, entries[i].message);
        }
    }
    
    if (max_count > 0) {
        printf("Most frequent message: '%s' (appears %d times)\n", most_frequent, max_count);
    }
}

void display_recent_entries(struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    printf("Recent log entries:\n");
    int display_count = (count > 5) ? 5 : count;
    
    for (int i = count - display_count; i < count; i++) {
        printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Example: 2024-01-15T10:30:00 INFO User logged in\n");
    printf("Enter 'END' on a new line to finish input\n");
    
    char line[MAX_LINE_LENGTH];
    
    while (entry_count < MAX_ENTRIES) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Please use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries processed: %d\n\n", entry_count);
    
    analyze_log_levels(entries, entry_count);
    printf("\n");
    find_most_frequent_message(entries, entry_count);
    printf("\n");
    display_recent_entries(entries, entry_count);
    
    return 0;
}