//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    const char* ptr = line;
    int fields_parsed = 0;
    
    while (*ptr && isspace(*ptr)) ptr++;
    
    if (*ptr == '[') {
        ptr++;
        int i = 0;
        while (*ptr && *ptr != ']' && i < 31) {
            entry->timestamp[i++] = *ptr++;
        }
        entry->timestamp[i] = '\0';
        if (*ptr == ']') {
            ptr++;
            fields_parsed++;
        }
    }
    
    while (*ptr && isspace(*ptr)) ptr++;
    
    if (*ptr == '[') {
        ptr++;
        int i = 0;
        while (*ptr && *ptr != ']' && i < 15) {
            entry->level[i++] = *ptr++;
        }
        entry->level[i] = '\0';
        if (*ptr == ']') {
            ptr++;
            fields_parsed++;
        }
    }
    
    while (*ptr && isspace(*ptr)) ptr++;
    
    int i = 0;
    while (*ptr && *ptr != '\n' && i < 255) {
        entry->message[i++] = *ptr++;
    }
    entry->message[i] = '\0';
    
    return fields_parsed >= 2;
}

void analyze_log_patterns(LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    char common_words[10][32] = {0};
    int word_counts[10] = {0};
    
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].level, "ERROR")) error_count++;
        else if (strstr(entries[i].level, "WARN")) warning_count++;
        else if (strstr(entries[i].level, "INFO")) info_count++;
        
        char message_copy[256];
        strncpy(message_copy, entries[i].message, 255);
        message_copy[255] = '\0';
        
        char* word = strtok(message_copy, " \t\n.,;:!?");
        while (word != NULL) {
            if (strlen(word) > 3) {
                for (int j = 0; j < 10; j++) {
                    if (common_words[j][0] == '\0') {
                        strncpy(common_words[j], word, 31);
                        common_words[j][31] = '\0';
                        word_counts[j] = 1;
                        break;
                    } else if (strcmp(common_words[j], word) == 0) {
                        word_counts[j]++;
                        break;
                    }
                }
            }
            word = strtok(NULL, " \t\n.,;:!?");
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nMost common words:\n");
    for (int i = 0; i < 10 && common_words[i][0] != '\0'; i++) {
        printf("  %s: %d occurrences\n", common_words[i], word_counts[i]);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log lines (format: [timestamp] [level] message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char line[MAX_LINE_LENGTH];
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        memset(&entry, 0, sizeof(entry));
        
        if (parse_log_line(line, &entry)) {
            if (entry_count < MAX_ENTRIES) {
                entries[entry_count++] = entry;
                printf("Parsed: [%s] [%s] %s\n", entry.timestamp, entry.level, entry.message);
            }
        } else {
            printf("Failed to parse line: %s\n", line);
        }
    }
    
    if (entry_count > 0) {
        clock_t start = clock();
        analyze_log_patterns(entries, entry_count);
        clock_t end = clock();
        double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("\n