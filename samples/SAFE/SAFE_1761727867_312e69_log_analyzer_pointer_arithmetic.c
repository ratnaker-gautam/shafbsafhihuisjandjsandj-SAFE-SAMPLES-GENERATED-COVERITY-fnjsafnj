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

int read_log_entries(struct LogEntry *entries, size_t max_entries) {
    char line[MAX_LINE_LEN];
    size_t count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        struct LogEntry *entry = entries + count;
        char *ptr = line;
        
        while (*ptr && isspace(*ptr)) ptr++;
        if (!*ptr) continue;
        
        char *ts_end = strchr(ptr, ' ');
        if (!ts_end || (size_t)(ts_end - ptr) >= sizeof(entry->timestamp)) continue;
        strncpy(entry->timestamp, ptr, (size_t)(ts_end - ptr));
        entry->timestamp[ts_end - ptr] = '\0';
        
        ptr = ts_end + 1;
        while (*ptr && isspace(*ptr)) ptr++;
        if (!*ptr) continue;
        
        char *level_end = strchr(ptr, ' ');
        if (!level_end || (size_t)(level_end - ptr) >= sizeof(entry->level)) continue;
        strncpy(entry->level, ptr, (size_t)(level_end - ptr));
        entry->level[level_end - ptr] = '\0';
        
        if (!is_valid_level(entry->level)) continue;
        
        ptr = level_end + 1;
        while (*ptr && isspace(*ptr)) ptr++;
        if (!*ptr) continue;
        
        size_t msg_len = strlen(ptr);
        if (msg_len > 0 && ptr[msg_len - 1] == '\n') ptr[--msg_len] = '\0';
        if (msg_len >= sizeof(entry->message)) msg_len = sizeof(entry->message) - 1;
        strncpy(entry->message, ptr, msg_len);
        entry->message[msg_len] = '\0';
        
        struct tm tm;
        if (!parse_timestamp(entry->timestamp, &tm)) continue;
        
        count++;
    }
    
    return count;
}

void analyze_logs(struct LogEntry *entries, size_t count) {
    size_t info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    for (size_t i = 0; i < count; i++) {
        struct LogEntry *entry = entries + i;
        if (strcmp(entry->level, "INFO") == 0) info_count++;
        else if (strcmp(entry->level, "WARN") == 0) warn_count++;
        else if (strcmp(entry->level, "ERROR") == 0) error_count++;
        else if (strcmp(entry->level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", count);
    printf("INFO:  %zu\n", info_count);
    printf("WARN:  %zu\n", warn_count);
    printf("ERROR: %zu\n", error_count);
    printf("DEBUG: %zu\n", debug_count);
    
    if (count > 0) {
        printf("\nRecent entries:\n");
        size_t show_count = count > 5 ? 5 : count;
        for (size_t i = count - show_count; i < count; i++) {
            struct LogEntry *entry = entries + i;
            printf("%s [%s] %s\n", entry->timestamp, entry->level, entry->message);
        }
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Press Ctrl+D to finish input\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    if (count <= 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(entries, (size_t)count);
    
    return 0;
}