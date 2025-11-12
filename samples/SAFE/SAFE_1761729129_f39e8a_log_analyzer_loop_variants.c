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

int parse_severity(const char* level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 0;
    return -1;
}

int read_log_entries(struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed == 3) {
            strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
            strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
            strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
            entries[count].severity = parse_severity(level);
            if (entries[count].severity >= 0) {
                count++;
            }
        }
    }
    
    return count;
}

void analyze_by_severity(struct LogEntry* entries, int count) {
    int severity_counts[4] = {0};
    int i = 0;
    
    while (i < count) {
        if (entries[i].severity >= 0 && entries[i].severity < 4) {
            severity_counts[entries[i].severity]++;
        }
        i++;
    }
    
    printf("Severity Analysis:\n");
    printf("DEBUG: %d\n", severity_counts[0]);
    printf("INFO:  %d\n", severity_counts[1]);
    printf("WARN:  %d\n", severity_counts[2]);
    printf("ERROR: %d\n", severity_counts[3]);
}

void find_common_messages(struct LogEntry* entries, int count) {
    char common_messages[10][256] = {0};
    int message_counts[10] = {0};
    int message_count = 0;
    int i, j;
    
    for (i = 0; i < count; i++) {
        int found = 0;
        for (j = 0; j < message_count; j++) {
            if (strcmp(entries[i].message, common_messages[j]) == 0) {
                message_counts[j]++;
                found = 1;
                break;
            }
        }
        if (!found && message_count < 10) {
            strncpy(common_messages[message_count], entries[i].message, sizeof(common_messages[message_count]) - 1);
            message_counts[message_count] = 1;
            message_count++;
        }
    }
    
    printf("\nCommon Messages (top %d):\n", message_count);
    for (i = 0; i < message_count; i++) {
        printf("%d: %s\n", message_counts[i], common_messages[i]);
    }
}

void show_recent_entries(struct LogEntry* entries, int count) {
    int show_count = (count > 5) ? 5 : count;
    int start = count - show_count;
    int i = start;
    
    printf("\nMost Recent %d Entries:\n", show_count);
    do {
        if (i >= 0 && i < count) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
        i++;
    } while (i < count);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (timestamp level message):\n");
    printf("Example: 2024-01-15T10:30:00 INFO User logged in\n");
    printf("Press Ctrl+D to finish input\n\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("Analyzed %d log entries:\n\n", count);
    
    analyze_by_severity(entries, count);
    find_common_messages(entries, count);
    show_recent_entries(entries, count);
    
    return 0;
}