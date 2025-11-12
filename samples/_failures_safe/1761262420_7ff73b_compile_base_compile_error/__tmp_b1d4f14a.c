//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
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
    
    entry->error_count = 0;
    if (strcmp(level, "ERROR") == 0) {
        entry->error_count = 1;
    }
    
    return 1;
}

int validate_log_entry(const LogEntry* entry) {
    if (entry == NULL) return 0;
    
    if (strlen(entry->timestamp) == 0) return 0;
    if (strlen(entry->level) == 0) return 0;
    if (strlen(entry->message) == 0) return 0;
    
    const char* valid_levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int valid = 0;
    for (int i = 0; i < 4; i++) {
        if (strcmp(entry->level, valid_levels[i]) == 0) {
            valid = 1;
            break;
        }
    }
    if (!valid) return 0;
    
    return 1;
}

void analyze_log_patterns(LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int info_count = 0;
    int warn_count = 0;
    int error_count = 0;
    int debug_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Level Summary:\n");
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    int consecutive_errors = 0;
    int max_consecutive = 0;
    for (int i = 0; i < count; i++) {
        if (entries[i].error_count > 0) {
            consecutive_errors++;
            if (consecutive_errors > max_consecutive) {
                max_consecutive = consecutive_errors;
            }
        } else {
            consecutive_errors = 0;
        }
    }
    
    printf("Maximum consecutive errors: %d\n", max_consecutive);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (one per line, format: timestamp level message):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LENGTH];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (validate_log_entry(&entry)) {
                entries[entry_count] = entry;
                entry_count++;
            } else {
                printf("Invalid log entry format: %s\n", line);
            }
        } else {
            printf("Failed to parse log entry: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nAnalyzing %d log entries...\n", entry_count);
    
    clock_t start_time = clock();
    
    analyze_log_patterns(entries, entry_count);
    
    int total_processing = 0;
    for (int i = 0; i < 1000000; i++) {
        for (int j = 0; j < entry_count; j++)