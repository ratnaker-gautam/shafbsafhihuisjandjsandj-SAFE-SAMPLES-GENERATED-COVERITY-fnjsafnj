//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LEVEL_LEN 10

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN - 3];
} LogEntry;

#define VALID_LEVEL(level) (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || strcmp(level, "ERROR") == 0)

int parse_timestamp(const char* str, struct tm* tm) {
    return sscanf(str, "%4d-%2d-%2d %2d:%2d:%2d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int is_valid_message(const char* msg) {
    if (msg == NULL || strlen(msg) == 0) return 0;
    for (size_t i = 0; msg[i] != '\0'; i++) {
        if (!isprint((unsigned char)msg[i]) && msg[i] != '\n') return 0;
    }
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        char timestamp[TIMESTAMP_LEN];
        char level[LEVEL_LEN];
        char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN - 3];
        
        int parsed = sscanf(line, "%19s %9s %255[^\n]", timestamp, level, message);
        
        if (parsed < 3) {
            printf("Invalid log format. Skipping: %s\n", line);
            continue;
        }
        
        struct tm time_struct;
        if (!parse_timestamp(timestamp, &time_struct)) {
            printf("Invalid timestamp. Skipping: %s\n", line);
            continue;
        }
        
        if (!VALID_LEVEL(level)) {
            printf("Invalid log level. Skipping: %s\n", line);
            continue;
        }
        
        if (!is_valid_message(message)) {
            printf("Invalid message content. Skipping: %s\n", line);
            continue;
        }
        
        LogEntry* entry = &entries[entry_count];
        strncpy(entry->timestamp, timestamp, TIMESTAMP_LEN - 1);
        entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
        strncpy(entry->level, level, LEVEL_LEN - 1);
        entry->level[LEVEL_LEN - 1] = '\0';
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries processed.\n");
        return 0;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    printf("Total entries: %d\n", entry_count);
    
    int info_count = 0, warn_count = 0, error_count = 0;
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
    }
    
    printf("INFO: %d (%.1f%%)\n", info_count, (float)info_count / entry_count * 100);
    printf("WARN: %d (%.1f%%)\n", warn_count, (float)warn_count / entry_count * 100);
    printf("ERROR: %d (%.1f%%)\n", error_count, (float)error_count / entry_count * 100);
    
    printf("\nRecent entries (last 5):\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    for (int i = start; i < entry_count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}