//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char level[16];
    char message[MAX_LINE_LENGTH];
    int timestamp;
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return 0;
    if (strcmp(level_str, "INFO") == 0) return 1;
    if (strcmp(level_str, "WARNING") == 0) return 2;
    if (strcmp(level_str, "ERROR") == 0) return 3;
    if (strcmp(level_str, "CRITICAL") == 0) return 4;
    return -1;
}

void print_log_entry(const struct LogEntry* entry, int indent) {
    if (indent < 0 || indent > MAX_LEVELS) return;
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
    printf("[%s] %d: %s\n", entry->level, entry->timestamp, entry->message);
}

void process_log_recursive(struct LogEntry* entries, int start, int end, int current_level, int indent) {
    if (start > end || start < 0 || end < 0 || indent < 0 || indent >= MAX_LEVELS) return;
    
    for (int i = start; i <= end; i++) {
        int entry_level = parse_log_level(entries[i].level);
        if (entry_level < 0) continue;
        
        if (entry_level == current_level) {
            print_log_entry(&entries[i], indent);
            process_log_recursive(entries, i + 1, end, current_level + 1, indent + 1);
        }
    }
}

int read_log_entries(struct LogEntry* entries, int max_entries) {
    if (!entries || max_entries <= 0) return 0;
    
    const char* sample_logs[] = {
        "INFO 1000 System startup",
        "DEBUG 1001 Initializing modules",
        "INFO 1002 Module A loaded",
        "WARNING 1003 Low memory",
        "ERROR 1004 Disk full",
        "CRITICAL 1005 System halt",
        "INFO 1006 Recovery started",
        "DEBUG 1007 Checking integrity",
        "INFO 1008 System restored",
        NULL
    };
    
    int count = 0;
    for (int i = 0; sample_logs[i] != NULL && count < max_entries; i++) {
        char level[16] = {0};
        int timestamp = 0;
        char message[MAX_LINE_LENGTH] = {0};
        
        if (sscanf(sample_logs[i], "%15s %d %1023[^\n]", level, &timestamp, message) == 3) {
            if (strlen(level) < sizeof(entries[count].level) && 
                strlen(message) < sizeof(entries[count].message)) {
                strcpy(entries[count].level, level);
                strcpy(entries[count].message, message);
                entries[count].timestamp = timestamp;
                count++;
            }
        }
    }
    return count;
}

int main(void) {
    struct LogEntry entries[20];
    int entry_count = read_log_entries(entries, 20);
    
    if (entry_count <= 0) {
        printf("No log entries to process\n");
        return 1;
    }
    
    printf("Log Analysis (recursive structure):\n");
    printf("===================================\n");
    
    process_log_recursive(entries, 0, entry_count - 1, 0, 0);
    
    int level_counts[5] = {0};
    for (int i = 0; i < entry_count; i++) {
        int level = parse_log_level(entries[i].level);
        if (level >= 0 && level < 5) {
            level_counts[level]++;
        }
    }
    
    printf("\nLog Level Summary:\n");
    printf("DEBUG: %d\n", level_counts[0]);
    printf("INFO: %d\n", level_counts[1]);
    printf("WARNING: %d\n", level_counts[2]);
    printf("ERROR: %d\n", level_counts[3]);
    printf("CRITICAL: %d\n", level_counts[4]);
    
    return 0;
}