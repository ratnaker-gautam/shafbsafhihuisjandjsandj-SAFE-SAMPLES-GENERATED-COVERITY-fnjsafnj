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
    do {
        if (strcmp(logs[i].level, level) == 0) {
            total++;
        }
        i++;
    } while (i < count);
    return total;
}

void display_log_summary(struct LogEntry* logs, int count) {
    if (count <= 0) return;
    
    printf("Log Summary:\n");
    printf("Total entries: %d\n", count);
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_counts[4] = {0};
    
    for (int i = 0; i < 4; i++) {
        int j = 0;
        while (j < count) {
            if (strcmp(logs[j].level, levels[i]) == 0) {
                level_counts[i]++;
            }
            j++;
        }
    }
    
    for (int i = 0; i < 4; i++) {
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
    
    printf("\nRecent entries:\n");
    int display_count = (count < 5) ? count : 5;
    for (int i = 0; i < display_count; i++) {
        int idx = count - display_count + i;
        printf("[%s] %s: %s\n", logs[idx].timestamp, logs[idx].level, logs[idx].message);
    }
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: timestamp|level|message)\n");
    printf("Enter 'quit' to finish input\n");
    
    char input[MAX_LINE_LEN];
    int input_count = 0;
    
    while (input_count < MAX_ENTRIES) {
        printf("Log entry %d: ", input_count + 1);
        
        if (fgets(input, MAX_LINE_LEN, stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        if (parse_log_line(input, &entry)) {
            if (log_count < MAX_ENTRIES) {
                logs[log_count] = entry;
                log_count++;
                input_count++;
            } else {
                printf("Maximum log entries reached.\n");
                break;
            }
        } else {
            printf("Invalid format. Use: timestamp|level|message\n");
        }
    }
    
    if (log_count > 0) {
        display_log_summary(logs, log_count);
        
        const char* search_levels[] = {"ERROR", "WARN"};
        for (int i = 0; i < 2; i++) {
            int count = count_logs_by_level(logs, log_count, search_levels[i]);
            printf("\n%s entries: %d\n", search_levels[i], count);
        }
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}