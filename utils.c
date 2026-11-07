#include "utils.h"

void clearScreen(void) {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen(void) {
    printf("\nPress [Enter] to continue...");
    fflush(stdout);
    char buf[64];
    if (!fgets(buf, sizeof(buf), stdin)) {
        return;
    }
}

void printCollegeHeader(void) {
    printf("================================================================================\n");
    printf("            MAR ATHANASIOS COLLEGE FOR ADVANCED STUDIES TIRUVALLA\n");
    printf("                     DEPARTMENT OF COMPUTER APPLICATIONS\n");
    printf("                             MCA 2024-2026 BATCH\n");
    printf("                C PROJECT - GROUP NO: 2: BUS RESERVATION SYSTEM\n");
    printf("                   Faculty Coordinator: Ms. Kripa Thankachan\n");
    printf("================================================================================\n");
}

void printBanner(const char *subTitle) {
    printCollegeHeader();
    if (subTitle && strlen(subTitle) > 0) {
        printf(" >>> %s <<<\n", subTitle);
        printf("--------------------------------------------------------------------------------\n");
    }
}

void trimString(char *str) {
    if (!str) return;
    int len = (int)strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r' || isspace((unsigned char)str[len - 1]))) {
        str[--len] = '\0';
    }
    char *start = str;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

int getInt(const char *prompt, int minVal, int maxVal) {
    char line[128];
    int value;
    char extra;

    while (1) {
        printf("%s", prompt);
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) {
            continue;
        }
        trimString(line);
        if (strlen(line) == 0) {
            printf(" [!] Error: Input cannot be empty. Please enter a number between %d and %d.\n", minVal, maxVal);
            continue;
        }
        if (sscanf(line, "%d %c", &value, &extra) == 1) {
            if (value >= minVal && value <= maxVal) {
                return value;
            } else {
                printf(" [!] Error: Value out of range. Enter a number between %d and %d.\n", minVal, maxVal);
            }
        } else {
            printf(" [!] Error: Invalid numeric input. Please try again.\n");
        }
    }
}

void getString(const char *prompt, char *buffer, int maxLen, int allowEmpty) {
    char line[256];

    while (1) {
        printf("%s", prompt);
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) {
            continue;
        }
        trimString(line);
        if (!allowEmpty && strlen(line) == 0) {
            printf(" [!] Error: Input cannot be blank. Please try again.\n");
            continue;
        }
        if ((int)strlen(line) > maxLen) {
            printf(" [!] Warning: Input exceeds maximum length of %d characters. Truncating.\n", maxLen);
            line[maxLen] = '\0';
        }
        strncpy(buffer, line, maxLen);
        buffer[maxLen] = '\0';
        return;
    }
}

void getDate(const char *prompt, char *buffer, int maxLen) {
    char line[64];
    while (1) {
        printf("%s (YYYY-MM-DD or DD-MM-YYYY): ", prompt);
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) {
            continue;
        }
        trimString(line);
        if (strlen(line) == 0) {
            printf(" [!] Error: Date cannot be blank.\n");
            continue;
        }

        int y, m, d;
        int valid = 0;
        if (sscanf(line, "%d-%d-%d", &y, &m, &d) == 3) {
            if (y > 1900 && m >= 1 && m <= 12 && d >= 1 && d <= 31) {
                valid = 1; /* YYYY-MM-DD */
            } else if (d > 1900 && m >= 1 && m <= 12 && y >= 1 && y <= 31) {
                /* DD-MM-YYYY */
                valid = 1;
            }
        } else if (sscanf(line, "%d/%d/%d", &d, &m, &y) == 3) {
            if (y > 1900 && m >= 1 && m <= 12 && d >= 1 && d <= 31) {
                valid = 1;
            }
        }

        if (!valid) {
            printf(" [!] Warning: Please enter a recognizable date format (e.g. 2026-10-25).\n");
            if (!confirmAction(" Accept entered date anyway? (y/n): ")) {
                continue;
            }
        }

        strncpy(buffer, line, maxLen - 1);
        buffer[maxLen - 1] = '\0';
        return;
    }
}

int confirmAction(const char *prompt) {
    char line[64];
    while (1) {
        printf("%s", prompt);
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) {
            continue;
        }
        trimString(line);
        if (line[0] == 'Y' || line[0] == 'y') {
            return 1;
        } else if (line[0] == 'N' || line[0] == 'n') {
            return 0;
        } else {
            printf(" [!] Please enter 'y' for yes or 'n' for no.\n");
        }
    }
}
