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

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#define IS_VALID_LEVEL(c) ((c) == 'E' || (c) == 'W' || (c) == 'I' || (c) == 'D')
#define LEVEL_TO_INDEX(c) ((c) == 'E' ? 0 : (c) == 'W' ? 1 : (c) == 'I' ? 2 : 3)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 4];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    size_t len = strlen(line);
    if (len < TIMESTAMP_LEN + 3 || len >= MAX_LINE_LEN) return 0;
    
    if (line[TIMESTAMP_LEN - 1] != ' ' || line[TIMESTAMP_LEN] != '[') return 0;
    if (line[TIMESTAMP_LEN + 2] != ']' || line[TIMESTAMP_LEN + 3] != ' ') return 0;
    
    if (!IS_VALID_LEVEL(line[TIMESTAMP_LEN + 1])) return 0;
    
    memcpy(entry->timestamp, line, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    
    entry->level = line[TIMESTAMP_LEN + 1];
    
    size_t msg_start = TIMESTAMP_LEN + 4;
    size_t msg_len = len - msg_start;
    if (msg_len >= sizeof(entry->message)) return 0;
    
    memcpy(entry->message, line + msg_start, msg_len);
    entry->message[msg_len] = '\0';
    
    return 1;
}

void print_summary(int counts[LOG_LEVELS]) {
    const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
    const char* level_colors[] = {"\033[31m", "\033[33m", "\033[32m", "\033[34m"};
    
    printf("Log Level Summary:\n");
    printf("==================\n");
    
    int total = 0;
    for (int i = 0; i < LOG_LEVELS; i++) {
        total += counts[i];
    }
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        float percentage = total > 0 ? (counts[i] * 100.0f) / total : 0.0f;
        printf("%s%-6s\033[0m: %d (%.1f%%)\n", 
               level_colors[i], level_names[i], counts[i], percentage);
    }
    printf("Total entries: %d\n", total);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", argv[1]);
        return 1;
    }
    
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int level_counts[LOG_LEVELS] = {0};
    
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), file) && entry_count < MAX_ENTRIES) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (len == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (entry_count < MAX_ENTRIES) {
                entries[entry_count] = entry;
                level_counts[LEVEL_TO_INDEX(entry.level)]++;
                entry_count++;
            }
        }
    }
    
    if (ferror(file)) {
        fprintf(stderr, "Error reading file\n");
        fclose(file);
        return 1;
    }
    
    fclose(file);
    
    printf("Successfully parsed %d log entries\n\n", entry_count);
    print_summary(level_counts);
    
    printf("\nRecent entries:\n");
    printf("===============\n");
    
    int start = entry_count > 5 ? entry_count - 5 : 0;
    for (int i = start; i < entry_count; i++) {
        const char* color = "\033[0m";
        switch (entries[i].level) {
            case 'E': color = "\033[31m"; break;
            case 'W': color = "\033[33m"; break;
            case 'I': color = "\033[32m"; break;
            case 'D': color = "\033[34m"; break;
        }
        
        printf