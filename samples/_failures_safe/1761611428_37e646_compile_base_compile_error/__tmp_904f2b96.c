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
    const char *valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    for (size_t i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int parse_log_line(char *line, struct LogEntry *entry) {
    char *ptr = line;
    
    while (*ptr && isspace(*ptr)) ptr++;
    if (!*ptr) return 0;
    
    char *timestamp_end = ptr;
    while (*timestamp_end && !isspace(*timestamp_end)) timestamp_end++;
    if (!*timestamp_end) return 0;
    
    size_t timestamp_len = timestamp_end - ptr;
    if (timestamp_len >= sizeof(entry->timestamp)) return 0;
    memcpy(entry->timestamp, ptr, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    ptr = timestamp_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    if (!*ptr) return 0;
    
    char *level_end = ptr;
    while (*level_end && !isspace(*level_end)) level_end++;
    if (!*level_end) return 0;
    
    size_t level_len = level_end - ptr;
    if (level_len >= sizeof(entry->level)) return 0;
    memcpy(entry->level, ptr, level_len);
    entry->level[level_len] = '\0';
    
    if (!is_valid_level(entry->level)) return 0;
    
    ptr = level_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    if (!*ptr) return 0;
    
    size_t message_len = strlen(ptr);
    if (message_len > 0 && ptr[message_len - 1] == '\n') {
        message_len--;
    }
    if (message_len >= sizeof(entry->message)) return 0;
    memcpy(entry->message, ptr, message_len);
    entry->message[message_len] = '\0';
    
    return 1;
}

int compare_entries(const void *a, const void *b) {
    const struct LogEntry *entry_a = (const struct LogEntry *)a;
    const struct LogEntry *entry_b = (const struct LogEntry *)b;
    
    struct tm tm_a, tm_b;
    if (!parse_timestamp(entry_a->timestamp, &tm_a)) return 0;
    if (!parse_timestamp(entry_b->timestamp, &tm_b)) return 0;
    
    time_t time_a = mktime(&tm_a);
    time_t time_b = mktime(&tm_b);
    
    if (time_a < time_b) return -1;
    if (time_a > time_b) return 1;
    return 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (timestamp level message). Empty line to finish:\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n') break;
        
        if (parse_log_line(line, entries + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: YYYY-MM-DD HH:MM:SS LEVEL message\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    qsort(entries, entry_count, sizeof(struct LogEntry), compare_entries);
    
    printf("\nSorted log entries:\n");
    printf("===================\n");
    
    for (int i = 0; i < entry_count; i++) {
        struct LogEntry *entry = entries + i;
        printf("%s %s %s\n", entry->timestamp, entry->level, entry->message);
    }
    
    int info_count = 0, warning_count = 0, error_count = 0, debug_count = 0;
    for (int i = 0; i < entry_count; i++) {
        struct LogEntry *entry = entries + i;
        char *level = entry->level;
        
        if (strcmp(level, "INFO") == 0) info_count++;
        else if (strcmp(level, "WARNING") == 0) warning_count++;
        else if (strcmp(level, "ERROR") == 0) error_count++;
        else if (strcmp(level, "DEBUG") == 0) debug_count++;
    }
    
    printf("\nStatistics:\n");
    printf("===========\n");