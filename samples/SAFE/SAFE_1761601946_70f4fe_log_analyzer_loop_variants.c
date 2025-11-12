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
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* parts[3];
    char* token = strtok(temp_line, "|");
    int part_count = 0;
    
    while (token != NULL && part_count < 3) {
        parts[part_count] = token;
        part_count++;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
    strncpy(entry->timestamp, parts[0], 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, parts[1], 15);
    entry->level[15] = '\0';
    strncpy(entry->message, parts[2], 255);
    entry->message[255] = '\0';
    
    return 1;
}

int count_logs_by_level(struct LogEntry* logs, int count, const char* level) {
    int total = 0;
    int i = 0;
    while (i < count) {
        if (strcmp(logs[i].level, level) == 0) {
            total++;
        }
        i++;
    }
    return total;
}

void print_log_summary(struct LogEntry* logs, int count) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    int j;
    for (j = 0; j < level_count; j++) {
        int cnt = count_logs_by_level(logs, count, levels[j]);
        printf("%s: %d\n", levels[j], cnt);
    }
    
    printf("\nRecent entries:\n");
    int display_count = (count > 5) ? 5 : count;
    int k = count - display_count;
    do {
        if (k >= 0 && k < count) {
            printf("[%s] %s: %s\n", logs[k].timestamp, logs[k].level, logs[k].message);
        }
        k++;
    } while (k < count);
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    const char* sample_logs[] = {
        "2024-01-15 10:30:00|INFO|System started successfully",
        "2024-01-15 10:31:15|DEBUG|Initializing modules",
        "2024-01-15 10:32:00|WARN|High memory usage detected",
        "2024-01-15 10:33:45|INFO|User login: john_doe",
        "2024-01-15 10:34:20|ERROR|Database connection failed",
        "2024-01-15 10:35:00|INFO|Retrying connection",
        "2024-01-15 10:36:10|DEBUG|Connection established",
        "2024-01-15 10:37:00|INFO|Operation completed",
        "2024-01-15 10:38:30|WARN|Slow response time",
        "2024-01-15 10:39:00|INFO|System shutdown initiated"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    int i;
    for (i = 0; i < sample_count; i++) {
        if (log_count < MAX_ENTRIES) {
            if (parse_log_line(sample_logs[i], &logs[log_count])) {
                log_count++;
            }
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    print_log_summary(logs, log_count);
    
    printf("\nSearching for ERROR entries:\n");
    int found_errors = 0;
    int idx = 0;
    while (idx < log_count) {
        if (strcmp(logs[idx].level, "ERROR") == 0) {
            printf("Found error: %s - %s\n", logs[idx].timestamp, logs[idx].message);
            found_errors++;
        }
        idx++;
    }
    
    if (found_errors == 0) {
        printf("No ERROR entries found.\n");
    }
    
    return 0;
}