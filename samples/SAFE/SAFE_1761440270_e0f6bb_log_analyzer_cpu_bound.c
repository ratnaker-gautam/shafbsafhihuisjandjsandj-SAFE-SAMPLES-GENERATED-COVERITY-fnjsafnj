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
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
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
    
    char line[MAX_LINE_LEN];
    int line_count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && entry_count < MAX_ENTRIES) {
        line_count++;
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        if (parse_log_line(line, &entries[entry_count])) {
            total_errors += entries[entry_count].error_count;
            entry_count++;
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    qsort(entries, entry_count, sizeof(LogEntry), compare_entries);
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Total lines read: %d\n", line_count);
    printf("Total ERROR entries: %d\n", total_errors);
    
    if (entry_count > 0) {
        printf("\nFirst entry: %s %s %s\n", 
               entries[0].timestamp, entries[0].level, entries[0].message);
        printf("Last entry: %s %s %s\n", 
               entries[entry_count-1].timestamp, 
               entries[entry_count-1].level, 
               entries[entry_count-1].message);
    }
    
    int error_sequences = 0;
    int consecutive_errors = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].error_count > 0) {
            consecutive_errors++;
        } else {
            if (consecutive_errors >= 2) {
                error_sequences++;
            }
            consecutive_errors = 0;
        }
    }
    
    if (consecutive_errors >= 2) {
        error_sequences++;
    }
    
    printf("Error sequences (2+ consecutive): %d\n", error_sequences);
    
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
            strncpy(most_common_level, levels[i], sizeof(most_common_level) - 1);
            most_common_level[sizeof(most_common_level) - 1] = '\0';
        }
    }
    
    printf("Most common log level: %s (%d occurrences)\n", most_common_level, max_count);
    
    return 0;
}