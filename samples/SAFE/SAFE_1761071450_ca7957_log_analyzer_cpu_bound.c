//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
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
    
    char buffer[MAX_LINE_LENGTH];
    strncpy(buffer, line, MAX_LINE_LENGTH - 1);
    buffer[MAX_LINE_LENGTH - 1] = '\0';
    
    char* token = strtok(buffer, " ");
    if (token == NULL) return 0;
    
    strncpy(entry->timestamp, token, 31);
    entry->timestamp[31] = '\0';
    
    token = strtok(NULL, " ");
    if (token == NULL) return 0;
    
    strncpy(entry->level, token, 15);
    entry->level[15] = '\0';
    
    token = strtok(NULL, "\n");
    if (token == NULL) return 0;
    
    strncpy(entry->message, token, 255);
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
    
    int level_cmp = strcmp(entry_a->level, entry_b->level);
    if (level_cmp != 0) return level_cmp;
    
    return strcmp(entry_a->timestamp, entry_b->timestamp);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    clock_t start_time = clock();
    
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
    
    printf("\n=== LOG ANALYSIS RESULTS ===\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Total error mentions: %d\n", total_errors);
    printf("Warning entries: %d\n", warning_count);
    printf("Info entries: %d\n", info_count);
    
    printf("\n=== SORTED LOG ENTRIES ===\n");
    for (int i = 0; i < entry_count; i++) {
        printf("%s %s %s (errors: %d)\n", 
               entries[i].timestamp, 
               entries[i].level, 
               entries[i].message, 
               entries[i].error_count);
    }
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("\nProcessing time: %.6f seconds\n", cpu_time);
    
    return 0;
}