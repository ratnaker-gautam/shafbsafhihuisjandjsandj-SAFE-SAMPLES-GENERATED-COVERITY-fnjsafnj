//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: log_analyzer
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
    char message[MAX_LINE_LEN - 48];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* tokens[3];
    char* token = strtok(temp, " ");
    int count = 0;
    
    while (token && count < 3) {
        tokens[count++] = token;
        token = strtok(NULL, " ");
    }
    
    if (count < 3) return 0;
    
    if (strlen(tokens[0]) >= sizeof(entry->timestamp)) return 0;
    strncpy(entry->timestamp, tokens[0], sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    if (strlen(tokens[1]) >= sizeof(entry->level)) return 0;
    strncpy(entry->level, tokens[1], sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    size_t msg_len = strlen(line) - (strlen(entry->timestamp) + strlen(entry->level) + 2);
    if (msg_len >= sizeof(entry->message)) return 0;
    strncpy(entry->message, line + strlen(entry->timestamp) + strlen(entry->level) + 2, 
            sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (!entries || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (error_count > 0) {
        printf("\nRecent ERROR messages:\n");
        int printed = 0;
        for (int i = count - 1; i >= 0 && printed < 3; i--) {
            if (strcmp(entries[i].level, "ERROR") == 0) {
                printf("- %s: %s\n", entries[i].timestamp, entries[i].message);
                printed++;
            }
        }
    }
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char filename[256];
    
    printf("Enter log file name: ");
    if (!fgets(filename, sizeof(filename), stdin)) {
        printf("Error reading filename\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file: %s\n", filename);
        return 1;
    }
    
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), file) && entry_count < MAX_ENTRIES) {
        size_t line_len = strlen(line);
        if (line_len > 0 && line[line_len - 1] == '\n') {
            line[line_len - 1] = '\0';
        }
        
        if (line_len > 1) {
            if (parse_log_line(line, &entries[entry_count])) {
                entry_count++;
            }
        }
    }
    
    if (ferror(file)) {
        printf("Error reading file\n");
        fclose(file);
        return 1;
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count);
    
    return 0;
}