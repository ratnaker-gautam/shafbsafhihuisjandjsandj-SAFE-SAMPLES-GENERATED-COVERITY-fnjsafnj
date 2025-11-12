//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
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
    char line[MAX_LINE_LEN];
    
    printf("Log Analyzer - Processing log entries...\n");
    
    for (int i = 0; i < 5000 && entry_count < MAX_ENTRIES; i++) {
        snprintf(line, MAX_LINE_LEN, 
                "2024-01-%02d %02d:%02d:%02d %s Sample log message %d", 
                (i % 30) + 1, 
                (i % 24), 
                (i % 60), 
                (i % 60),
                (i % 10 == 0) ? "ERROR" : ((i % 5 == 0) ? "WARNING" : "INFO"),
                i);
        
        if (parse_log_line(line, &entries[entry_count])) {
            if (strcmp(entries[entry_count].level, "ERROR") == 0) {
                total_errors++;
            } else if (strcmp(entries[entry_count].level, "WARNING") == 0) {
                total_warnings++;
            }
            entry_count++;
        }
    }
    
    printf("Analysis Results:\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Total errors: %d\n", total_errors);
    printf("Total warnings: %d\n", total_warnings);
    printf("Error rate: %.2f%%\n", entry_count > 0 ? (total_errors * 100.0) / entry_count : 0.0);
    
    int error_sequences = 0;
    int current_sequence = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].error_count > 0) {
            current_sequence++;
        } else {
            if (current_sequence >= 3) {
                error_sequences++;
            }
            current_sequence = 0;
        }
    }
    
    if (current_sequence >= 3) {
        error_sequences++;
    }
    
    printf("Error sequences (3+ consecutive): %d\n", error_sequences);
    
    char most_common_level[16] = "INFO";
    int info_count = entry_count - total_errors - total_warnings;
    
    if (total_errors > info_count && total_errors > total_warnings) {
        strcpy(most_common_level, "ERROR");
    } else if (total_warnings > info_count && total_warnings > total_errors) {
        strcpy(most_common_level, "WARNING");
    }
    
    printf("Most common log level: %s\n", most_common_level);
    
    return 0;
}