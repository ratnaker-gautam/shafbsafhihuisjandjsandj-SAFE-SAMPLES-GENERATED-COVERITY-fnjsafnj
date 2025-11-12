//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_timestamp(const char *str, struct tm *tm) {
    return sscanf(str, "%d-%d-%d %d:%d:%d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int is_valid_level(const char *level) {
    const char *valid[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    for (size_t i = 0; i < 4; i++) {
        if (strcmp(level, valid[i]) == 0) return 1;
    }
    return 0;
}

int read_log_entry(FILE *file, struct LogEntry *entry) {
    char line[MAX_LINE_LEN];
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    char *ptr = line;
    while (*ptr && isspace(*ptr)) ptr++;
    
    char *timestamp_end = strchr(ptr, ' ');
    if (!timestamp_end || (timestamp_end - ptr) >= 32) return 0;
    strncpy(entry->timestamp, ptr, timestamp_end - ptr);
    entry->timestamp[timestamp_end - ptr] = '\0';
    
    ptr = timestamp_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    
    char *level_end = strchr(ptr, ' ');
    if (!level_end || (level_end - ptr) >= 16) return 0;
    strncpy(entry->level, ptr, level_end - ptr);
    entry->level[level_end - ptr] = '\0';
    
    if (!is_valid_level(entry->level)) return 0;
    
    ptr = level_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    
    size_t msg_len = strlen(ptr);
    if (msg_len == 0 || msg_len >= 256) return 0;
    if (ptr[msg_len - 1] == '\n') ptr[msg_len - 1] = '\0';
    strncpy(entry->message, ptr, 255);
    entry->message[255] = '\0';
    
    return 1;
}

void analyze_logs(struct LogEntry *logs, size_t count) {
    int level_counts[4] = {0};
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    struct tm earliest = {0}, latest = {0};
    int first = 1;
    
    for (size_t i = 0; i < count; i++) {
        struct LogEntry *entry = logs + i;
        
        for (int j = 0; j < 4; j++) {
            if (strcmp(entry->level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
        
        struct tm current;
        if (parse_timestamp(entry->timestamp, &current)) {
            time_t current_time = mktime(&current);
            if (first) {
                earliest = latest = current;
                first = 0;
            } else {
                time_t earliest_time = mktime(&earliest);
                time_t latest_time = mktime(&latest);
                if (current_time < earliest_time) earliest = current;
                if (current_time > latest_time) latest = current;
            }
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", count);
    for (int i = 0; i < 4; i++) {
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
    if (!first) {
        printf("Time range: %s to %s\n", 
               asctime(&earliest), asctime(&latest));
    }
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    size_t count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter empty line to finish.\n");
    
    char line[MAX_LINE_LEN];
    while (count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n') break;
        
        FILE *temp = tmpfile();
        if (!temp) {
            fprintf(stderr, "Error creating temporary file\n");
            return 1;
        }
        
        fputs(line, temp);
        rewind(temp);
        
        if (read_log_entry(temp, logs + count)) {
            count++;
        } else {
            fprintf(stderr, "Invalid log entry format\n");
        }
        
        fclose(temp);
    }
    
    if (count > 0) {
        analyze_logs(logs, count);
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}