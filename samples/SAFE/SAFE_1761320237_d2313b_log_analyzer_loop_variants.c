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
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* parts[3];
    parts[0] = temp;
    int part_count = 1;
    
    for (char* p = temp; *p && part_count < 3; p++) {
        if (*p == '|') {
            *p = '\0';
            parts[part_count] = p + 1;
            part_count++;
        }
    }
    
    if (part_count != 3) return 0;
    
    if (strlen(parts[0]) >= 19) return 0;
    strncpy(entry->timestamp, parts[0], 19);
    entry->timestamp[19] = '\0';
    
    if (strlen(parts[1]) >= 9) return 0;
    strncpy(entry->level, parts[1], 9);
    entry->level[9] = '\0';
    
    if (strlen(parts[2]) >= MAX_LINE_LEN - 31) return 0;
    strncpy(entry->message, parts[2], MAX_LINE_LEN - 31);
    entry->message[MAX_LINE_LEN - 31] = '\0';
    
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

void display_logs(struct LogEntry* logs, int count, const char* filter_level) {
    if (count <= 0) return;
    
    int i;
    for (i = 0; i < count; i++) {
        if (!filter_level || strcmp(logs[i].level, filter_level) == 0) {
            printf("%s | %s | %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
        }
    }
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char input[MAX_LINE_LEN];
    
    printf("Log Analyzer - Enter log lines (format: timestamp|level|message)\n");
    printf("Enter 'done' to finish input, 'stats' for statistics, 'filter level' to filter\n");
    
    while (1) {
        printf("> ");
        if (!fgets(input, MAX_LINE_LEN, stdin)) break;
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "done") == 0) {
            break;
        }
        else if (strcmp(input, "stats") == 0) {
            int error_count = count_logs_by_level(logs, log_count, "ERROR");
            int warn_count = count_logs_by_level(logs, log_count, "WARN");
            int info_count = count_logs_by_level(logs, log_count, "INFO");
            
            printf("Statistics:\n");
            printf("ERROR: %d\n", error_count);
            printf("WARN:  %d\n", warn_count);
            printf("INFO:  %d\n", info_count);
            printf("TOTAL: %d\n", log_count);
        }
        else if (strncmp(input, "filter ", 7) == 0) {
            const char* level = input + 7;
            if (strcmp(level, "ERROR") == 0 || strcmp(level, "WARN") == 0 || strcmp(level, "INFO") == 0) {
                printf("Filtered logs (%s):\n", level);
                display_logs(logs, log_count, level);
            } else {
                printf("Invalid level. Use ERROR, WARN, or INFO.\n");
            }
        }
        else {
            if (log_count >= MAX_ENTRIES) {
                printf("Maximum log entries reached.\n");
                continue;
            }
            
            if (parse_log_line(input, &logs[log_count])) {
                log_count++;
                printf("Log entry added.\n");
            } else {
                printf("Invalid log format. Use: timestamp|level|message\n");
            }
        }
    }
    
    printf("Final log summary (%d entries):\n", log_count);
    display_logs(logs, log_count, NULL);
    
    return 0;
}