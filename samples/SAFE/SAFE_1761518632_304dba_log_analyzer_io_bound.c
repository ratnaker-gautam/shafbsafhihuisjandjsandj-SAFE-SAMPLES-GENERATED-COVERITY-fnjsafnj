//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
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
    
    return 1;
}

int count_entries_by_level(const struct LogEntry* entries, int count, const char* level) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char input_line[MAX_LINE_LEN];
    
    printf("Log Analyzer - Enter log lines (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
            break;
        }
        
        input_line[strcspn(input_line, "\n")] = '\0';
        
        if (strcmp(input_line, "END") == 0) {
            break;
        }
        
        if (strlen(input_line) == 0) {
            continue;
        }
        
        struct LogEntry new_entry;
        if (parse_log_line(input_line, &new_entry)) {
            entries[entry_count] = new_entry;
            entry_count++;
        } else {
            printf("Invalid log format. Skipping line.\n");
        }
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total log entries: %d\n", entry_count);
    
    if (entry_count > 0) {
        int info_count = count_entries_by_level(entries, entry_count, "INFO");
        int warn_count = count_entries_by_level(entries, entry_count, "WARN");
        int error_count = count_entries_by_level(entries, entry_count, "ERROR");
        
        printf("INFO entries: %d\n", info_count);
        printf("WARN entries: %d\n", warn_count);
        printf("ERROR entries: %d\n", error_count);
        
        printf("\nRecent entries:\n");
        int display_count = (entry_count < 5) ? entry_count : 5;
        for (int i = 0; i < display_count; i++) {
            int idx = entry_count - display_count + i;
            printf("%s [%s] %s\n", entries[idx].timestamp, entries[idx].level, entries[idx].message);
        }
    }
    
    return 0;
}