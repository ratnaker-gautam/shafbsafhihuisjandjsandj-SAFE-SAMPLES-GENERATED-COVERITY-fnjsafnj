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

const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

int parse_log_level(const char *str) {
    if (!str) return -1;
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strncmp(str, level_names[i], strlen(level_names[i])) == 0) {
            return i;
        }
    }
    return -1;
}

int parse_timestamp(const char *str, char *timestamp) {
    if (!str || !timestamp) return 0;
    int i = 0;
    while (str[i] && i < TIMESTAMP_LEN - 1 && IS_TIMESTAMP_CHAR(str[i])) {
        timestamp[i] = str[i];
        i++;
    }
    timestamp[i] = '\0';
    return i > 0;
}

int read_log_entry(FILE *file, LogEntry *entry) {
    if (!file || !entry) return 0;
    char line[MAX_LINE_LEN];
    if (!fgets(line, sizeof(line), file)) return 0;
    
    size_t len = strlen(line);
    if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';
    
    if (!parse_timestamp(line, entry->timestamp)) return 0;
    
    char *level_start = strchr(line, ']');
    if (!level_start || level_start - line >= MAX_LINE_LEN - 1) return 0;
    level_start++;
    while (*level_start == ' ') level_start++;
    
    char *level_end = strchr(level_start, ' ');
    if (!level_end) return 0;
    
    char level_str[16];
    size_t level_len = level_end - level_start;
    if (level_len >= sizeof(level_str)) return 0;
    strncpy(level_str, level_start, level_len);
    level_str[level_len] = '\0';
    
    entry->level = parse_log_level(level_str);
    if (!IS_VALID_LEVEL(entry->level)) return 0;
    
    char *msg_start = level_end + 1;
    while (*msg_start == ' ') msg_start++;
    
    if (strlen(msg_start) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, msg_start);
    
    return 1;
}

void analyze_logs(LogEntry *entries, int count) {
    if (!entries || count <= 0) return;
    
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", LEVEL_NAME(i), level_counts[i], 
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: [timestamp] LEVEL message)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR\n");
    printf("Enter empty line to finish.\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        printf("Log entry %d: ", entry_count + 1);
        if (!fgets(line, sizeof(line), stdin)) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';
        if (strlen(line) == 0) break;
        
        FILE *temp = tmpfile();
        if (!temp) continue;
        fputs(line, temp);
        rewind(temp);
        
        if (read_log_entry(temp, &entries[entry_count])) {
            entry_count++;
            printf("  Added: [%s] %s %s\n", 
                   entries[entry_count-1].timestamp,
                   LEVEL_NAME(entries[entry_count-1].level),
                   entries[entry_count-1].message);
        } else {
            printf("  Invalid format. Use: [YYYY-MM-DD HH:MM:SS] LEVEL message\n");
        }
        
        fclose(temp);
    }
    
    if (entry_count > 0) {
        analyze_log