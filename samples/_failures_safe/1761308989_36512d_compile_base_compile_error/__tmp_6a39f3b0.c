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
    
    char buffer[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(buffer, line, MAX_LINE_LENGTH - 1);
    buffer[MAX_LINE_LENGTH - 1] = '\0';
    
    char* parts[3];
    parts[0] = strtok(buffer, "|");
    if (parts[0] == NULL) return 0;
    
    for (int i = 1; i < 3; i++) {
        parts[i] = strtok(NULL, "|");
        if (parts[i] == NULL) return 0;
    }
    
    if (strlen(parts[0]) >= sizeof(entry->timestamp) - 1) return 0;
    if (strlen(parts[1]) >= sizeof(entry->level) - 1) return 0;
    if (strlen(parts[2]) >= sizeof(entry->message) - 1) return 0;
    
    strncpy(entry->timestamp, parts[0], sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, parts[1], sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, parts[2], sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int read_log_file(struct LogEntry entries[], int max_entries) {
    printf("Enter log data (format: timestamp|level|message, empty line to finish):\n");
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (count < max_entries) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[count])) {
            count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    return count;
}

void analyze_logs(const struct LogEntry entries[], int count) {
    if (entries == NULL || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (error_count > 0) {
        printf("\nERROR entries found:\n");
        for (int i = 0; i < count; i++) {
            if (strcmp(entries[i].level, "ERROR") == 0) {
                printf("- %s: %s\n", entries[i].timestamp, entries[i].message);
            }
        }
    }
}

int validate_log_level(const char* level) {
    if (level == NULL) return 0;
    return (strcmp(level, "ERROR") == 0 || 
            strcmp(level, "WARNING") == 0 || 
            strcmp(level, "INFO") == 0);
}

void filter_by_level(const struct LogEntry entries[], int count, const char* level) {
    if (entries == NULL || count <= 0 || !validate_log_level(level)) return;
    
    printf("\nEntries with level %s:\n", level);
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            printf("- %s | %s | %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No entries found with level %s\n", level);
    }
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    
    int entry_count = read_log_file(entries, MAX_ENTRIES);
    
    if (entry_count == 0) {