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
    const char *valid_levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    for (size_t i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) return 1;
    }
    return 0;
}

int read_log_entries(FILE *file, struct LogEntry *entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (count < max_entries && fgets(line, sizeof(line), file)) {
        char *ptr = line;
        
        while (*ptr && isspace(*ptr)) ptr++;
        if (!*ptr) continue;
        
        struct LogEntry *entry = entries + count;
        
        char *timestamp_end = strchr(ptr, ' ');
        if (!timestamp_end || timestamp_end - ptr >= 32) continue;
        strncpy(entry->timestamp, ptr, timestamp_end - ptr);
        entry->timestamp[timestamp_end - ptr] = '\0';
        
        ptr = timestamp_end + 1;
        while (*ptr && isspace(*ptr)) ptr++;
        
        char *level_end = strchr(ptr, ' ');
        if (!level_end || level_end - ptr >= 16) continue;
        strncpy(entry->level, ptr, level_end - ptr);
        entry->level[level_end - ptr] = '\0';
        
        if (!is_valid_level(entry->level)) continue;
        
        ptr = level_end + 1;
        while (*ptr && isspace(*ptr)) ptr++;
        
        size_t msg_len = strlen(ptr);
        if (msg_len > 0 && ptr[msg_len - 1] == '\n') ptr[--msg_len] = '\0';
        if (msg_len >= 256) msg_len = 255;
        strncpy(entry->message, ptr, msg_len);
        entry->message[msg_len] = '\0';
        
        count++;
    }
    
    return count;
}

void analyze_logs(struct LogEntry *entries, int count) {
    int level_counts[4] = {0};
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    for (int i = 0; i < count; i++) {
        struct LogEntry *entry = entries + i;
        for (int j = 0; j < 4; j++) {
            if (strcmp(entry->level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < 4; i++) {
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
        return 1;
    }
    
    struct LogEntry entries[MAX_ENTRIES];
    int count = read_log_entries(file, entries, MAX_ENTRIES);
    fclose(file);
    
    if (count == 0) {
        fprintf(stderr, "Error: No valid log entries found\n");
        return 1;
    }
    
    analyze_logs(entries, count);
    
    return 0;
}