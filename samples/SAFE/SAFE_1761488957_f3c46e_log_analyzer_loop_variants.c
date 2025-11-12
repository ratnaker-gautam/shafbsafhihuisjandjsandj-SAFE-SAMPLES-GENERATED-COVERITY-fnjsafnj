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
    if (hours <= 0) return;
    
    time_t now = time(NULL);
    time_t cutoff = now - (hours * 3600);
    
    printf("Entries from last %d hours:\n", hours);
    
    int found = 0;
    for (int i = 0; i < count; i++) {
        struct tm tm_time;
        if (strptime(entries[i].timestamp, "%Y-%m-%dT%H:%M:%S", &tm_time) != NULL) {
            time_t entry_time = mktime(&tm_time);
            if (entry_time >= cutoff) {
                printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
                found = 1;
            }
        }
    }
    
    if (!found) {
        printf("No recent entries found.\n");
    }
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DDTHH:MM:SS LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
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
            printf("%s %s\n", entries[error_index].timestamp, entries[error_index].message);
        }
        error_index++;
    } while (error_index < entry_count);
    
    return 0;
}