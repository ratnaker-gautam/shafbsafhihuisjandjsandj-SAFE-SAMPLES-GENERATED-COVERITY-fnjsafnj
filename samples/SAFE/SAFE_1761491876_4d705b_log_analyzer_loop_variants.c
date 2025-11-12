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

int is_valid_level(const char* level) {
    const char* valid_levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    size_t i = 0;
    while (i < 4) {
        if (strcmp(level, valid_levels[i]) == 0) return 1;
        i++;
    }
    return 0;
}

void analyze_logs(struct LogEntry* logs, int count) {
    int level_counts[4] = {0};
    const char* level_names[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    int i = 0;
    do {
        int j = 0;
        while (j < 4) {
            if (strcmp(logs[i].level, level_names[j]) == 0) {
                level_counts[j]++;
                break;
            }
            j++;
        }
        i++;
    } while (i < count);
    
    printf("Log Level Summary:\n");
    int k;
    for (k = 0; k < 4; k++) {
        printf("%s: %d entries\n", level_names[k], level_counts[k]);
    }
    
    printf("\nRecent ERROR entries:\n");
    int error_count = 0;
    int idx = count - 1;
    while (idx >= 0 && error_count < 5) {
        if (strcmp(logs[idx].level, "ERROR") == 0) {
            printf("- %s: %s\n", logs[idx].timestamp, logs[idx].message);
            error_count++;
        }
        idx--;
    }
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    printf("Enter log entries (format: timestamp|level|message):\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    char line[MAX_LINE_LEN];
    int line_num = 1;
    
    while (log_count < MAX_ENTRIES) {
        printf("Entry %d: ", line_num);
        
        if (fgets(line, MAX_LINE_LEN, stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) break;
        
        if (len == 0 || len >= MAX_LINE_LEN - 1) {
            printf("Invalid line length. Skipping.\n");
            line_num++;
            continue;
        }
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (is_valid_level(entry.level)) {
                logs[log_count] = entry;
                log_count++;
                printf("Added: %s | %s | %s\n", entry.timestamp, entry.level, entry.message);
            } else {
                printf("Invalid log level. Skipping.\n");
            }
        } else {
            printf("Invalid format. Use: timestamp|level|message\n");
        }
        
        line_num++;
    }
    
    if (log_count > 0) {
        printf("\nAnalysis Results:\n");
        analyze_logs(logs, log_count);
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}