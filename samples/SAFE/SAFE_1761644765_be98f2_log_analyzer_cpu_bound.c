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
    
    char* token = strtok(temp_line, " ");
    if (!token) return 0;
    
    strncpy(entry->timestamp, token, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    token = strtok(NULL, " ");
    if (!token) return 0;
    
    strncpy(entry->level, token, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    token = strtok(NULL, "\n");
    if (!token) return 0;
    
    strncpy(entry->message, token, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    entry->error_count = 0;
    const char* ptr = entry->message;
    while (*ptr) {
        if (strncmp(ptr, "error", 5) == 0 && !isalnum(ptr[5])) {
            entry->error_count++;
        }
        ptr++;
    }
    
    return 1;
}

int compare_entries(const void* a, const void* b) {
    const LogEntry* entry_a = (const LogEntry*)a;
    const LogEntry* entry_b = (const LogEntry*)b;
    return strcmp(entry_a->timestamp, entry_b->timestamp);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n' || line[0] == '\0') break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    qsort(entries, entry_count, sizeof(LogEntry), compare_entries);
    
    int total_errors = 0;
    int warning_count = 0;
    int info_count = 0;
    
    printf("\nSorted log analysis:\n");
    printf("===================\n");
    
    for (int i = 0; i < entry_count; i++) {
        printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        
        if (strcmp(entries[i].level, "ERROR") == 0) {
            total_errors++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
        
        total_errors += entries[i].error_count;
    }
    
    printf("\nSummary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR level entries: %d\n", total_errors);
    printf("WARNING level entries: %d\n", warning_count);
    printf("INFO level entries: %d\n", info_count);
    
    if (entry_count > 0) {
        double error_rate = (double)total_errors / entry_count * 100.0;
        printf("Error rate: %.2f%%\n", error_rate);
    }
    
    return 0;
}