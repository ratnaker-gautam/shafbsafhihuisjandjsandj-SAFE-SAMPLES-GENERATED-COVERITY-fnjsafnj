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
    int part_count = 0;
    char* token = strtok(temp_line, "|");
    
    while (token != NULL && part_count < 3) {
        parts[part_count++] = token;
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
    if (count <= 0) return;
    
    printf("Log Summary:\n");
    printf("Total entries: %d\n", count);
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    int j;
    
    for (j = 0; j < level_count; j++) {
        int cnt = count_logs_by_level(logs, count, levels[j]);
        printf("%s: %d\n", levels[j], cnt);
    }
    
    printf("\nRecent entries:\n");
    int start_idx = (count > 5) ? count - 5 : 0;
    int k = start_idx;
    
    do {
        printf("[%s] %s: %s\n", logs[k].timestamp, logs[k].level, logs[k].message);
        k++;
    } while (k < count && k < start_idx + 5);
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char input_line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: timestamp|level|message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (log_count < MAX_ENTRIES) {
        if (fgets(input_line, MAX_LINE_LEN, stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_line);
        if (len > 0 && input_line[len - 1] == '\n') {
            input_line[len - 1] = '\0';
        }
        
        if (strcmp(input_line, "END") == 0) {
            break;
        }
        
        if (strlen(input_line) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        if (parse_log_line(input_line, &entry)) {
            logs[log_count] = entry;
            log_count++;
        } else {
            printf("Invalid log format. Use: YYYY-MM-DD HH:MM:SS|LEVEL|message\n");
        }
    }
    
    if (log_count > 0) {
        print_log_summary(logs, log_count);
    } else {
        printf("No valid log entries processed.\n");
    }
    
    return 0;
}