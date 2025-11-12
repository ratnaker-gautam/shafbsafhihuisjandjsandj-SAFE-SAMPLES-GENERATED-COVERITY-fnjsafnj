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
        if (strncmp(str, level_names[i], strlen(level_names[i])) == 0) return i;
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
    char line[MAX_LINE_LEN];
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    size_t len = strlen(line);
    if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';
    
    if (!parse_timestamp(line, entry->timestamp)) return 0;
    
    char *level_start = line + strlen(entry->timestamp);
    while (*level_start == ' ' && *level_start != '\0') level_start++;
    if (*level_start == '\0') return 0;
    
    char *bracket = strchr(level_start, ']');
    if (bracket == NULL) return 0;
    
    size_t level_len = bracket - level_start - 1;
    if (level_len <= 0 || level_len >= 20) return 0;
    
    char level_str[20];
    strncpy(level_str, level_start + 1, level_len);
    level_str[level_len] = '\0';
    
    entry->level = parse_log_level(level_str);
    if (!IS_VALID_LEVEL(entry->level)) return 0;
    
    char *message_start = bracket + 1;
    while (*message_start == ' ' && *message_start != '\0') message_start++;
    if (*message_start == '\0') return 0;
    
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int level_counts[LOG_LEVELS] = {0};
    
    printf("Enter log data (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES) {
        LogEntry entry;
        if (!read_log_entry(stdin, &entry)) {
            char buffer[MAX_LINE_LEN];
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                if (strlen(buffer) == 1 && buffer[0] == '\n') break;
            } else {
                break;
            }
            continue;
        }
        
        entries[entry_count] = entry;
        if (IS_VALID_LEVEL(entry.level)) {
            level_counts[entry.level]++;
        }
        entry_count++;
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("Level distribution:\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("  %s: %d\n", LEVEL_NAME(i), level_counts[i]);
    }
    
    printf("\nRecent entries (last 5):\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    for (int i = start; i < entry_count; i++) {
        printf("[%s] [%s] %s\n", entries[i].timestamp, LEVEL_NAME(entries[i].level), entries[i].message);
    }
    
    int error_count = level_counts[3];
    if (error_count > 0) {
        printf("\nALERT: Found %d ERROR entries!\n", error_count);
    } else {
        printf("\nNo errors detected.\n");
    }
    
    return 0;
}