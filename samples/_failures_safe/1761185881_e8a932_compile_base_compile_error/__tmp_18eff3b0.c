//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)
#define LEVEL_NAME(l) (level_names[(l)])
#define IS_TIMESTAMP_CHAR(c) (isdigit((c)) || (c) == '-' || (c) == ':' || (c) == ' ' || (c) == '.')

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

static const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

int parse_log_level(const char *str) {
    if (str == NULL) return -1;
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strncmp(str, level_names[i], strlen(level_names[i])) == 0) {
            return i;
        }
    }
    return -1;
}

int parse_timestamp(const char *str, char *timestamp) {
    if (str == NULL || timestamp == NULL) return 0;
    int i = 0;
    while (i < TIMESTAMP_LEN - 1 && str[i] != '\0' && IS_TIMESTAMP_CHAR(str[i])) {
        timestamp[i] = str[i];
        i++;
    }
    timestamp[i] = '\0';
    return i > 0;
}

int read_log_entry(FILE *file, LogEntry *entry) {
    if (file == NULL || entry == NULL) return 0;
    char buffer[MAX_LINE_LEN];
    if (fgets(buffer, sizeof(buffer), file) == NULL) return 0;
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
    
    if (!parse_timestamp(buffer, entry->timestamp)) return 0;
    
    char *level_start = buffer + strlen(entry->timestamp);
    while (*level_start == ' ' || *level_start == '\t') level_start++;
    
    char *level_end = level_start;
    while (*level_end != '\0' && *level_end != ' ' && *level_end != '\t' && *level_end != '[') level_end++;
    
    if (level_end == level_start) return 0;
    
    char level_str[16];
    size_t level_len = level_end - level_start;
    if (level_len >= sizeof(level_str)) level_len = sizeof(level_str) - 1;
    strncpy(level_str, level_start, level_len);
    level_str[level_len] = '\0';
    
    entry->level = parse_log_level(level_str);
    if (entry->level == -1) return 0;
    
    char *message_start = level_end;
    while (*message_start == ' ' || *message_start == '\t' || *message_start == ']' || *message_start == '[') message_start++;
    
    if (strlen(message_start) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, message_start);
    
    return 1;
}

void analyze_logs(LogEntry *entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int level_counts[LOG_LEVELS] = {0};
    int total_entries = 0;
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
            total_entries++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries processed: %d\n", total_entries);
    printf("Level distribution:\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        float percentage = total_entries > 0 ? (level_counts[i] * 100.0f) / total_entries : 0.0f;
        printf("  %s: %d (%.1f%%)\n", LEVEL_NAME(i), level_counts[i], percentage);
    }
    
    printf("\nRecent entries:\n");
    int show_count = count < 5 ? count : 5;
    for (int i = 0; i < show_count; i++) {
        printf("[%s] %s: %s\n", entries[i].timestamp, LEVEL_NAME(entries[i].level), entries[i].message);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log entries (empty line to finish):\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL Message\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR\n\n");
    
    while (entry_count < MAX_ENTRIES) {
        printf("Log entry %d: ", entry_count + 1);
        
        LogEntry entry;
        if (!read_log_entry(stdin, &entry)) {
            char buffer[MAX_LINE_LEN];
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                size_t