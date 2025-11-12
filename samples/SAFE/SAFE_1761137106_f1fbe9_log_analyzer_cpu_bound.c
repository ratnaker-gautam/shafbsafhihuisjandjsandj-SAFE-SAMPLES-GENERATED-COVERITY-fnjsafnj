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
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* tokens[3];
    char* token = strtok(temp_line, "|");
    int token_count = 0;
    
    while (token != NULL && token_count < 3) {
        tokens[token_count] = token;
        token_count++;
        token = strtok(NULL, "|");
    }
    
    if (token_count != 3) return 0;
    
    strncpy(entry->timestamp, tokens[0], 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, tokens[1], 15);
    entry->level[15] = '\0';
    strncpy(entry->message, tokens[2], 255);
    entry->message[255] = '\0';
    
    entry->error_count = 0;
    const char* msg_ptr = entry->message;
    while (*msg_ptr) {
        if (strncmp(msg_ptr, "error", 5) == 0) {
            entry->error_count++;
        }
        msg_ptr++;
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
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: timestamp|level|message), empty line to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LEN, stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: timestamp|level|message\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    qsort(entries, entry_count, sizeof(LogEntry), compare_entries);
    
    int total_errors = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        total_errors += entries[i].error_count;
        
        if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO entries: %d\n", info_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("Total error mentions: %d\n", total_errors);
    
    printf("\nSorted log entries by timestamp:\n");
    for (int i = 0; i < entry_count && i < 10; i++) {
        printf("%s | %s | %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    if (entry_count > 10) {
        printf("... and %d more entries\n", entry_count - 10);
    }
    
    return 0;
}