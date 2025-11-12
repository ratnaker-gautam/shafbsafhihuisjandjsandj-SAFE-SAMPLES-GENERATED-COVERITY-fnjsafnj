//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char* token = strtok(temp_line, " ");
    if (!token) return 0;
    
    if (strlen(token) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, token);
    
    token = strtok(NULL, " ");
    if (!token) return 0;
    
    if (strlen(token) >= sizeof(entry->level)) return 0;
    strcpy(entry->level, token);
    
    token = strtok(NULL, "\n");
    if (!token) return 0;
    
    if (strlen(token) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, token);
    
    if (strcmp(entry->level, "ERROR") == 0) entry->severity = 3;
    else if (strcmp(entry->level, "WARN") == 0) entry->severity = 2;
    else if (strcmp(entry->level, "INFO") == 0) entry->severity = 1;
    else entry->severity = 0;
    
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char input_line[MAX_LINE_LEN];
    
    printf("Enter log entries (one per line, format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(input_line, sizeof(input_line), stdin)) break;
        
        input_line[strcspn(input_line, "\n")] = 0;
        
        if (strcmp(input_line, "END") == 0) break;
        
        if (strlen(input_line) == 0) continue;
        
        if (parse_log_line(input_line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", input_line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    int other_count = 0;
    
    int i = 0;
    while (i < entry_count) {
        if (entries[i].severity == 3) error_count++;
        else if (entries[i].severity == 2) warn_count++;
        else if (entries[i].severity == 1) info_count++;
        else other_count++;
        i++;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARN entries: %d\n", warn_count);
    printf("INFO entries: %d\n", info_count);
    printf("Other entries: %d\n", other_count);
    
    int max_severity = 0;
    for (int j = 0; j < entry_count; j++) {
        if (entries[j].severity > max_severity) {
            max_severity = entries[j].severity;
        }
    }
    
    printf("\nHighest severity level found: ");
    switch (max_severity) {
        case 3: printf("ERROR\n"); break;
        case 2: printf("WARN\n"); break;
        case 1: printf("INFO\n"); break;
        default: printf("UNKNOWN\n"); break;
    }
    
    printf("\nRecent log entries (last 5):\n");
    int start_idx = (entry_count > 5) ? entry_count - 5 : 0;
    int k = start_idx;
    do {
        printf("%s [%s] %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
        k++;
    } while (k < entry_count);
    
    return 0;
}