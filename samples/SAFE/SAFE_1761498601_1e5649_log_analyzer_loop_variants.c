//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
#define MAX_ENTRIES 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    int i = 0;
    while (temp[i] != '\0' && i < 19) {
        if (temp[i] == ' ') break;
        entry->timestamp[i] = temp[i];
        i++;
    }
    entry->timestamp[i] = '\0';
    
    while (temp[i] == ' ' && temp[i] != '\0') i++;
    
    int j = 0;
    while (temp[i] != '\0' && temp[i] != ' ' && j < 9) {
        entry->level[j] = temp[i];
        i++;
        j++;
    }
    entry->level[j] = '\0';
    
    while (temp[i] == ' ' && temp[i] != '\0') i++;
    
    j = 0;
    while (temp[i] != '\0' && j < MAX_LINE_LENGTH - 31) {
        entry->message[j] = temp[i];
        i++;
        j++;
    }
    entry->message[j] = '\0';
    
    return 1;
}

void analyze_log_levels(struct LogEntry* entries, int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int k = 0;
    do {
        if (strcmp(entries[k].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[k].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[k].level, "INFO") == 0) {
            info_count++;
        }
        k++;
    } while (k < count);
    
    printf("Log Level Summary:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARNING: %d\n", warning_count);
    printf("INFO: %d\n", info_count);
}

void find_recent_entries(struct LogEntry* entries, int count) {
    printf("\nRecent log entries:\n");
    
    int start = count > 5 ? count - 5 : 0;
    for (int i = start; i < count; i++) {
        printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish.\n");
    
    char line[MAX_LINE_LENGTH];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry new_entry;
        if (parse_log_line(line, &new_entry)) {
            entries[entry_count] = new_entry;
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    analyze_log_levels(entries, entry_count);
    find_recent_entries(entries, entry_count);
    
    return 0;
}