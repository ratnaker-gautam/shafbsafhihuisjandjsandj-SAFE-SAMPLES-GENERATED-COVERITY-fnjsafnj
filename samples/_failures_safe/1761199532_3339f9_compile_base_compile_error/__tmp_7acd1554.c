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
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) {
        return 0;
    }
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
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
    int total_errors = 0;
    
    char line[MAX_LINE_LEN];
    int line_count = 0;
    
    printf("Enter log entries (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        line_count++;
        
        if (line[0] == '\n' || line[0] == '\0') break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &entries[entry_count])) {
            total_errors += entries[entry_count].error_count;
            entry_count++;
        } else {
            printf("Invalid log format on line %d\n", line_count);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries processed.\n");
        return 1;
    }
    
    qsort(entries, entry_count, sizeof(LogEntry), compare_entries);
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Total errors found: %d\n", total_errors);
    printf("Error rate: %.2f%%\n", (total_errors * 100.0) / entry_count);
    
    printf("\nFirst 10 entries (sorted by timestamp):\n");
    int display_count = (entry_count < 10) ? entry_count : 10;
    for (int i = 0; i < display_count; i++) {
        printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    char most_common_level[16] = "INFO";
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) debug_count++;
    }
    
    int max_count = info_count;
    strcpy(most_common_level, "INFO");
    
    if (warn_count > max_count) {
        max_count = warn_count;
        strcpy(most_common_level, "WARN");
    }
    if (error_count > max_count) {
        max_count = error_count;
        strcpy(most_common_level, "ERROR");
    }
    if (debug_count > max_count) {
        max_count = debug_count;
        strcpy(most_common_level, "DEBUG");
    }
    
    printf("\nLog level distribution:\n");
    printf("INFO: %d (%.1f%%)\n", info_count, (info_count * 100.0) / entry_count);
    printf("WARN: %d (%.1f%%)\n", warn_count, (warn_count * 100.0) / entry_count);
    printf("ERROR: %d (%.1f%%)\n", error_count, (error_count * 100.0) / entry_count);
    printf("DEBUG: %d (%.1f%%)\n", debug_count, (debug_count * 100.0) / entry_count);
    printf("Most common level: %s\n", most_common_level);