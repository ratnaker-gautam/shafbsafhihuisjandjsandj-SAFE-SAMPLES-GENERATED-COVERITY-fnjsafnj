//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    char level_buf[16];
    char message_buf[256];
    char timestamp_buf[32];
    
    if (strlen(line) >= MAX_LINE_LEN - 1) {
        return 0;
    }
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", 
                       timestamp_buf, level_buf, message_buf);
    
    if (parsed != 3) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp_buf, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level_buf, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message_buf, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_log_levels(struct LogEntry* entries, int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    int i = 0;
    
    while (i < count) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
        i++;
    }
    
    printf("Log Level Summary:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARNING: %d\n", warning_count);
    printf("INFO: %d\n", info_count);
}

void find_recent_entries(struct LogEntry* entries, int count, int hours) {
    if (hours <= 0) {
        printf("Invalid time window\n");
        return;
    }
    
    time_t now = time(NULL);
    time_t cutoff = now - (hours * 3600);
    
    printf("Entries from last %d hours:\n", hours);
    
    int found = 0;
    for (int i = 0; i < count; i++) {
        struct tm tm_time;
        if (strptime(entries[i].timestamp, "%Y-%m-%dT%H:%M:%S", &tm_time) != NULL) {
            time_t entry_time = mktime(&tm_time);
            if (entry_time >= cutoff) {
                printf("%s %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
                found = 1;
            }
        }
    }
    
    if (!found) {
        printf("No entries found in specified time window\n");
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Type 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided\n");
        return 1;
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    analyze_log_levels(entries, entry_count);
    printf("\n");
    
    find_recent_entries(entries, entry_count, 24);
    
    int error_index = 0;
    printf("\nError entries:\n");
    do {
        if (strcmp(entries[error_index].level, "ERROR") == 0) {
            printf("%s: %s\n", entries[error_index].timestamp, entries[error_index].message);
        }
        error_index++;
    } while (error_index < entry_count);
    
    return 0;
}