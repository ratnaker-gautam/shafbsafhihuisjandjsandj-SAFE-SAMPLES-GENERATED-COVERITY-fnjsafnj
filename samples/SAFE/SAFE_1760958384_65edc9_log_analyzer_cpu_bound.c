//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
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
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    entry->error_count = 0;
    
    const char* msg_ptr = entry->message;
    while (*msg_ptr) {
        if (strncasecmp(msg_ptr, "error", 5) == 0 && !isalnum(*(msg_ptr + 5))) {
            entry->error_count++;
        }
        msg_ptr++;
    }
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    int total_warnings = 0;
    
    char line[MAX_LINE_LEN];
    int line_count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && entry_count < MAX_ENTRIES) {
        line_count++;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (len >= MAX_LINE_LEN - 1) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
            
            if (strcmp(entry.level, "ERROR") == 0) {
                total_errors++;
            } else if (strcmp(entry.level, "WARN") == 0) {
                total_warnings++;
            }
            
            total_errors += entry.error_count;
        }
    }
    
    if (line_count == 0) {
        fprintf(stderr, "No input data provided\n");
        return 1;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total lines processed: %d\n", line_count);
    printf("Valid log entries: %d\n", entry_count);
    printf("ERROR level entries: %d\n", total_errors);
    printf("WARN level entries: %d\n", total_warnings);
    
    int error_messages = 0;
    for (int i = 0; i < entry_count; i++) {
        error_messages += entries[i].error_count;
    }
    printf("Error mentions in messages: %d\n", error_messages);
    
    char most_common_level[16] = "UNKNOWN";
    int level_counts[3] = {0};
    const char* levels[] = {"INFO", "WARN", "ERROR"};
    
    for (int i = 0; i < entry_count; i++) {
        for (int j = 0; j < 3; j++) {
            if (strcmp(entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    int max_count = 0;
    for (int i = 0; i < 3; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            strcpy(most_common_level, levels[i]);
        }
    }
    
    printf("Most common log level: %s (%d occurrences)\n", most_common_level, max_count);
    
    return 0;
}