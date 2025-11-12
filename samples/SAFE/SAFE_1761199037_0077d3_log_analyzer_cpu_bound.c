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
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
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
    
    entry->error_count = 0;
    if (strcmp(level, "ERROR") == 0) {
        entry->error_count = 1;
    }
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    int total_warnings = 0;
    
    printf("Log Analyzer - Enter log lines (empty line to finish):\n");
    
    char line[MAX_LINE_LENGTH];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            if (strcmp(entries[entry_count].level, "ERROR") == 0) {
                total_errors++;
            } else if (strcmp(entries[entry_count].level, "WARNING") == 0) {
                total_warnings++;
            }
            entry_count++;
        } else {
            printf("Invalid log format. Expected: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Total errors: %d\n", total_errors);
    printf("Total warnings: %d\n", total_warnings);
    printf("Error rate: %.2f%%\n", entry_count > 0 ? (total_errors * 100.0) / entry_count : 0.0);
    
    printf("\n=== Error Summary ===\n");
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].error_count > 0) {
            printf("%s: %s\n", entries[i].timestamp, entries[i].message);
        }
    }
    
    printf("\n=== Performance Statistics ===\n");
    int info_count = 0;
    int debug_count = 0;
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(entries[i].level, "DEBUG") == 0) {
            debug_count++;
        }
    }
    printf("INFO entries: %d\n", info_count);
    printf("DEBUG entries: %d\n", debug_count);
    
    if (entry_count > 0) {
        printf("Most frequent level: ");
        int max_count = 0;
        char most_frequent[16] = "UNKNOWN";
        
        const char* levels[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
        int counts[] = {total_errors, total_warnings, info_count, debug_count};
        
        for (int i = 0; i < 4; i++) {
            if (counts[i] > max_count) {
                max_count = counts[i];
                strcpy(most_frequent, levels[i]);
            }
        }
        printf("%s (%d occurrences)\n", most_frequent, max_count);
    }
    
    return 0;
}