//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries\r", count);
    fflush(stdout);
}

void find_errors(LogEntry* entry) {
    if (strstr(entry->level, "ERROR") != NULL) {
        printf("ERROR found: %s - %s\n", entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    if (strstr(entry->level, "WARN") != NULL) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

void process_log_file(const char* filename, LogProcessor processor) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    LogEntry entry;
    int line_count = 0;

    while (fgets(line, sizeof(line), file) && line_count < MAX_ENTRIES) {
        if (strlen(line) < 20) continue;

        memset(&entry, 0, sizeof(entry));
        
        char* timestamp_end = strchr(line, ' ');
        if (!timestamp_end) continue;
        
        size_t timestamp_len = timestamp_end - line;
        if (timestamp_len >= sizeof(entry.timestamp)) continue;
        strncpy(entry.timestamp, line, timestamp_len);
        entry.timestamp[timestamp_len] = '\0';

        char* level_start = timestamp_end + 1;
        char* level_end = strchr(level_start, ' ');
        if (!level_end) continue;
        
        size_t level_len = level_end - level_start;
        if (level_len >= sizeof(entry.level)) continue;
        strncpy(entry.level, level_start, level_len);
        entry.level[level_len] = '\0';

        char* message_start = level_end + 1;
        size_t message_len = strlen(message_start);
        if (message_len > 0 && message_start[message_len-1] == '\n') {
            message_start[message_len-1] = '\0';
        }
        if (message_len >= sizeof(entry.message)) {
            message_len = sizeof(entry.message) - 1;
        }
        strncpy(entry.message, message_start, message_len);
        entry.message[message_len] = '\0';

        processor(&entry);
        line_count++;
    }

    if (ferror(file)) {
        fprintf(stderr, "Error reading file\n");
    }

    fclose(file);
    printf("\n");
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    printf("Log Analysis Menu:\n");
    printf("1. Count entries\n");
    printf("2. Find errors\n");
    printf("3. Show warnings\n");
    printf("4. All operations\n");
    printf("Choose option (1-4): ");

    char input[16];
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    int choice = atoi(input);
    if (choice < 1 || choice > 4) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }

    LogProcessor processors[] = {count_entries, find_errors, show_warnings};
    const char* names[] = {"Counting", "Error search", "Warning display"};

    if (choice == 4) {
        for (int i = 0; i < 3; i++) {
            printf("\n--- %s ---\n", names[i]);
            process_log_file(argv[1], processors[i]);
        }
    } else {
        process_log_file(argv[1], processors[choice-1]);
    }

    return 0;
}