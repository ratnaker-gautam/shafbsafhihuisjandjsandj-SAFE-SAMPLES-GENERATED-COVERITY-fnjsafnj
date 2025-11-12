//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define IS_VALID_LEVEL(level) \
    (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || \
     strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0)

#define PARSE_TIMESTAMP(ts, tm) \
    (strptime(ts, "%Y-%m-%d %H:%M:%S", tm) != NULL)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[8];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 12];
} LogEntry;

int read_log_file(const char* filename, LogEntry entries[], int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) >= TIMESTAMP_LEN + 3) {
            char* ts_start = line;
            char* level_start = ts_start + TIMESTAMP_LEN + 1;
            
            if (level_start - line < sizeof(line)) {
                char level_buf[8] = {0};
                strncpy(level_buf, level_start, 7);
                level_buf[7] = '\0';
                
                if (IS_VALID_LEVEL(level_buf)) {
                    strncpy(entries[count].timestamp, ts_start, TIMESTAMP_LEN - 1);
                    entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
                    
                    strncpy(entries[count].level, level_buf, 7);
                    entries[count].level[7] = '\0';
                    
                    char* msg_start = level_start + strlen(level_buf) + 1;
                    if (msg_start - line < sizeof(line)) {
                        size_t msg_len = strlen(line) - (msg_start - line);
                        if (msg_len > 0 && msg_len < sizeof(entries[count].message)) {
                            strncpy(entries[count].message, msg_start, msg_len);
                            entries[count].message[msg_len] = '\0';
                            char* newline = strchr(entries[count].message, '\n');
                            if (newline) *newline = '\0';
                            count++;
                        }
                    }
                }
            }
        }
    }
    
    fclose(file);
    return count;
}

void analyze_logs(LogEntry entries[], int count) {
    int level_counts[LOG_LEVELS] = {0};
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < LOG_LEVELS; j++) {
            if (strcmp(entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", levels[i], level_counts[i], 
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
    
    if (count > 0) {
        struct tm first_tm = {0}, last_tm = {0};
        if (PARSE_TIMESTAMP(entries[0].timestamp, &first_tm) && 
            PARSE_TIMESTAMP(entries[count-1].timestamp, &last_tm)) {
            time_t first = mktime(&first_tm);
            time_t last = mktime(&last_tm);
            if (first != -1 && last != -1) {
                double hours = difftime(last, first) / 3600.0;
                printf("Time span: %.2f hours\n", hours);
            }
        }
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log file name: ");
    char filename[256];
    if (fgets(filename, sizeof(filename), stdin)) {
        char* newline = strchr(filename, '\n');
        if (newline) *newline = '\0';
        
        if (strlen(filename) > 0) {
            int count = read_log_file(filename, entries, MAX_ENTRIES);
            if (count > 0) {
                analyze_logs(entries, count);
            } else {
                printf("Error: Could not read log file or no valid entries found.\n");
            }
        } else {
            printf("Error: Empty filename.\n");
        }
    } else {
        printf("Error: Failed to read input.\n");
    }
    
    return 0;
}