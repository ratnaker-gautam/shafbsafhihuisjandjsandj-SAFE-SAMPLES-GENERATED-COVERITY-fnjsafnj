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

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    int total_warnings = 0;
    
    char line[MAX_LINE_LEN];
    int line_count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && entry_count < MAX_ENTRIES) {
        line_count++;
        if (strlen(line) >= MAX_LINE_LEN - 1) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
            
            if (strcmp(entry.level, "ERROR") == 0) {
                total_errors++;
            } else if (strcmp(entry.level, "WARNING") == 0) {
                total_warnings++;
            }
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    qsort(entries, entry_count, sizeof(LogEntry), compare_entries);
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Total ERROR entries: %d\n", total_errors);
    printf("Total WARNING entries: %d\n", total_warnings);
    printf("Lines with 'error' in message: ");
    
    int error_in_message_count = 0;
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].error_count > 0) {
            error_in_message_count++;
        }
    }
    printf("%d\n", error_in_message_count);
    
    printf("\nFirst 5 entries (sorted by timestamp):\n");
    int display_count = (entry_count < 5) ? entry_count : 5;
    for (int i = 0; i < display_count; i++) {
        printf("%s %s %s (errors in message: %d)\n", 
               entries[i].timestamp, entries[i].level, 
               entries[i].message, entries[i].error_count);
    }
    
    printf("\nLast 5 entries (sorted by timestamp):\n");
    int start_idx = (entry_count > 5) ? entry_count - 5 : 0;
    for (int i = start_idx; i < entry_count; i++) {
        printf("%s %s %s (errors in message: %d)\n", 
               entries[i].timestamp, entries[i].level, 
               entries[i].message, entries[i].error_count);
    }
    
    return 0;
}