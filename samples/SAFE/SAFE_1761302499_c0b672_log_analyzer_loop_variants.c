//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* parts[3];
    parts[0] = temp;
    int part_count = 1;
    
    for (int i = 0; temp[i] != '\0' && part_count < 3; i++) {
        if (temp[i] == ' ' && part_count < 3) {
            temp[i] = '\0';
            parts[part_count] = &temp[i + 1];
            part_count++;
        }
    }
    
    if (part_count != 3) return 0;
    
    if (strlen(parts[0]) >= 19 || strlen(parts[1]) >= 9 || strlen(parts[2]) >= MAX_LINE_LEN - 30) {
        return 0;
    }
    
    strncpy(entry->timestamp, parts[0], 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, parts[1], 9);
    entry->level[9] = '\0';
    strncpy(entry->message, parts[2], MAX_LINE_LEN - 31);
    entry->message[MAX_LINE_LEN - 31] = '\0';
    
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
    
    time_t now;
    time(&now);
    time_t cutoff = now - (hours * 3600);
    
    printf("Entries from last %d hours:\n", hours);
    
    for (int i = 0; i < count; i++) {
        struct tm tm_time;
        if (strptime(entries[i].timestamp, "%Y-%m-%d %H:%M:%S", &tm_time) != NULL) {
            time_t entry_time = mktime(&tm_time);
            if (entry_time != -1 && entry_time >= cutoff) {
                printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            }
        }
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    analyze_log_levels(entries, entry_count);
    printf("\n");
    
    int hours;
    printf("Enter hours to search for recent entries: ");
    if (scanf("%d", &hours) != 1 || hours <= 0) {
        printf("Invalid input for hours.\n");
        return 1;
    }
    
    find_recent_entries(entries, entry_count, hours);
    
    return 0;
}