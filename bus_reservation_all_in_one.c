/*
 * ============================================================================
 * MAR ATHANASIOS COLLEGE FOR ADVANCED STUDIES TIRUVALLA (MACFAST)
 * DEPARTMENT OF COMPUTER APPLICATIONS
 * MCA 2024-2026 BATCH - C PROJECT
 * Group No: 2 | Project: Bus Reservation System
 * Faculty Coordinator: Ms. Kripa Thankachan
 * ============================================================================
 * Features:
 *  - BUS FILE operations (Insert, Edit, Delete, View)
 *  - PASSENGER FILE operations (Insert, Edit, Delete, View)
 *  - BUS RESERVATION FILE operations (Insert, Edit, Delete, View, Ticket)
 *  - Binary file handling (bus.dat, passenger.dat, reservation.dat)
 *  - Dynamic seat availability tracking
 *  - Printable Boarding Pass / Ticket generator
 *  - Demo Data Seeder for instant evaluation
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUS_FILE "bus.dat"
#define PASSENGER_FILE "passenger.dat"
#define RESERVATION_FILE "reservation.dat"

/* ============================================================================
 * DATA STRUCTURE DEFINITIONS (AS PER PROJECT SPECIFICATION DOCUMENT)
 * ============================================================================
 */

/* 1. BUS FILE
 * b_id    : Int
 * b_name  : Char[20]
 * b_from  : Char[20]
 * b_to    : Char[20]
 * b_seats : Int
 * b_fare  : int
 */
typedef struct {
    int b_id;
    char b_name[21];
    char b_from[21];
    char b_to[21];
    int b_seats;
    int b_fare;
} Bus;

/* 2. PASSENGER FILE
 * p_id   : Int
 * p_name : Char[20]
 * p_addr : Char[40]
 */
typedef struct {
    int p_id;
    char p_name[21];
    char p_addr[41];
} Passenger;

/* 3. BUS RESERVATION FILE
 * r_id    : int
 * b_id    : int
 * p_id    : Char[25]
 * r_date  : Char[10]
 * r_seats : int
 */
typedef struct {
    int r_id;
    int b_id;
    char p_id[26];
    char r_date[12];
    int r_seats;
} Reservation;

/* ============================================================================
 * FUNCTION DECLARATIONS
 * ============================================================================
 */
void clearScreen(void);
void pauseScreen(void);
void printCollegeHeader(void);
void printBanner(const char *subTitle);
void trimString(char *str);
int getInt(const char *prompt, int minVal, int maxVal);
void getString(const char *prompt, char *buffer, int maxLen, int allowEmpty);
void getDate(const char *prompt, char *buffer, int maxLen);
int confirmAction(const char *prompt);

/* Bus Operations */
int insertBus(void);
int editBus(void);
int deleteBus(void);
void viewAllBuses(void);
void viewBusById(void);
int findBus(int b_id, Bus *outBus);
int busExists(int b_id);
int getBusCount(void);

/* Passenger Operations */
int insertPassenger(void);
int editPassenger(void);
int deletePassenger(void);
void viewAllPassengers(void);
void viewPassengerById(void);
int findPassenger(int p_id, Passenger *outPass);
int passengerExists(int p_id);
int getPassengerCount(void);

/* Reservation Operations */
int insertReservation(void);
int editReservation(void);
int deleteReservation(void);
void viewAllReservations(void);
void viewReservationById(void);
void printTicket(int r_id);
void checkBusAvailabilityUI(void);
int getBookedSeats(int b_id, const char *r_date, int exclude_r_id);
int getAvailableSeats(int b_id, const char *r_date, int exclude_r_id);
int reservationExists(int r_id);
int findReservation(int r_id, Reservation *outRes);
int getReservationCount(void);

/* Menu & Demo */
void busMenu(void);
void passengerMenu(void);
void reservationMenu(void);
void seedSampleData(void);
void showSystemSummary(void);

/* ============================================================================
 * MAIN FUNCTION & MENUS
 * ============================================================================
 */
int main(void) {
    int choice;

    while (1) {
        clearScreen();
        printBanner("MAIN SYSTEM MENU");
        printf("  1. BUS MANAGEMENT           (Insert, Edit, Delete, View)\n");
        printf("  2. PASSENGER MANAGEMENT     (Insert, Edit, Delete, View)\n");
        printf("  3. RESERVATION MANAGEMENT   (Book, Edit, Cancel, View)\n");
        printf("  4. GENERATE / PRINT TICKET  (Boarding Pass)\n");
        printf("  5. CHECK SEAT AVAILABILITY  (By Bus & Date)\n");
        printf("  6. LOAD SAMPLE DEMO DATA    (Seed Initial Records)\n");
        printf("  7. SYSTEM SUMMARY           (Record Counts & Files)\n");
        printf("  8. EXIT\n");
        printf("--------------------------------------------------------------------------------\n");

        choice = getInt("Select an option (1-8): ", 1, 8);

        switch (choice) {
            case 1:
                busMenu();
                break;
            case 2:
                passengerMenu();
                break;
            case 3:
                reservationMenu();
                break;
            case 4: {
                clearScreen();
                printBanner("PRINT TICKET");
                int r_id = getInt("Enter Reservation ID: ", 1, 999999);
                printTicket(r_id);
                pauseScreen();
                break;
            }
            case 5:
                checkBusAvailabilityUI();
                break;
            case 6:
                seedSampleData();
                break;
            case 7:
                showSystemSummary();
                break;
            case 8:
                clearScreen();
                printBanner("THANK YOU");
                printf("\n  Exiting Bus Reservation System.\n");
                printf("  Project by: MCA 2024-2026 Batch - Group No: 2\n");
                printf("  Department of Computer Applications\n");
                printf("  Mar Athanasios College for Advanced Studies Tiruvalla (MACFAST)\n");
                printf("  Faculty Coordinator: Ms. Kripa Thankachan\n\n");
                exit(0);
            default:
                break;
        }
    }
    return 0;
}

void busMenu(void) {
    int choice;
    while (1) {
        clearScreen();
        printBanner("BUS MANAGEMENT MENU");
        printf("  1. Insert a Record  (Add New Bus)\n");
        printf("  2. Edit a Record    (Modify Bus Details)\n");
        printf("  3. Delete a Record  (Remove Bus)\n");
        printf("  4. View All Records (List all Buses in Table)\n");
        printf("  5. View Record by ID\n");
        printf("  6. Back to Main Menu\n");
        printf("--------------------------------------------------------------------------------\n");

        choice = getInt("Enter your choice (1-6): ", 1, 6);

        switch (choice) {
            case 1: insertBus(); break;
            case 2: editBus(); break;
            case 3: deleteBus(); break;
            case 4: viewAllBuses(); break;
            case 5: viewBusById(); break;
            case 6: return;
            default: break;
        }
    }
}

void passengerMenu(void) {
    int choice;
    while (1) {
        clearScreen();
        printBanner("PASSENGER MANAGEMENT MENU");
        printf("  1. Insert a Record  (Add New Passenger)\n");
        printf("  2. Edit a Record    (Modify Passenger Details)\n");
        printf("  3. Delete a Record  (Remove Passenger)\n");
        printf("  4. View All Records (List all Passengers in Table)\n");
        printf("  5. View Record by ID\n");
        printf("  6. Back to Main Menu\n");
        printf("--------------------------------------------------------------------------------\n");

        choice = getInt("Enter your choice (1-6): ", 1, 6);

        switch (choice) {
            case 1: insertPassenger(); break;
            case 2: editPassenger(); break;
            case 3: deletePassenger(); break;
            case 4: viewAllPassengers(); break;
            case 5: viewPassengerById(); break;
            case 6: return;
            default: break;
        }
    }
}

void reservationMenu(void) {
    int choice;
    while (1) {
        clearScreen();
        printBanner("BUS RESERVATION MANAGEMENT MENU");
        printf("  1. Insert a Record  (Book New Reservation)\n");
        printf("  2. Edit a Record    (Modify Reservation/Seats/Date)\n");
        printf("  3. Delete a Record  (Cancel Reservation)\n");
        printf("  4. View All Records (List all Reservations)\n");
        printf("  5. View Record by ID (Boarding Pass / Ticket)\n");
        printf("  6. Back to Main Menu\n");
        printf("--------------------------------------------------------------------------------\n");

        choice = getInt("Enter your choice (1-6): ", 1, 6);

        switch (choice) {
            case 1: insertReservation(); break;
            case 2: editReservation(); break;
            case 3: deleteReservation(); break;
            case 4: viewAllReservations(); break;
            case 5: viewReservationById(); break;
            case 6: return;
            default: break;
        }
    }
}

/* ============================================================================
 * UTILITY & UI FUNCTIONS
 * ============================================================================
 */
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
            if (y > 1900 && m >= 1 && m <= 12 && d >= 1 && d <= 31) valid = 1;
            else if (d > 1900 && m >= 1 && m <= 12 && y >= 1 && y <= 31) valid = 1;
        } else if (sscanf(line, "%d/%d/%d", &d, &m, &y) == 3) {
            if (y > 1900 && m >= 1 && m <= 12 && d >= 1 && d <= 31) valid = 1;
        }

        if (!valid) {
            printf(" [!] Warning: Date format unrecognized (e.g. 2026-10-25).\n");
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
        if (line[0] == 'Y' || line[0] == 'y') return 1;
        if (line[0] == 'N' || line[0] == 'n') return 0;
        printf(" [!] Please enter 'y' for yes or 'n' for no.\n");
    }
}

/* ============================================================================
 * BUS FILE OPERATIONS
 * ============================================================================
 */
int busExists(int b_id) {
    FILE *fp = fopen(BUS_FILE, "rb");
    if (!fp) return 0;
    Bus b;
    while (fread(&b, sizeof(Bus), 1, fp) == 1) {
        if (b.b_id == b_id) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int findBus(int b_id, Bus *outBus) {
    FILE *fp = fopen(BUS_FILE, "rb");
    if (!fp) return 0;
    Bus b;
    while (fread(&b, sizeof(Bus), 1, fp) == 1) {
        if (b.b_id == b_id) {
            if (outBus) *outBus = b;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int getBusCount(void) {
    FILE *fp = fopen(BUS_FILE, "rb");
    if (!fp) return 0;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return (int)(size / sizeof(Bus));
}

int insertBus(void) {
    clearScreen();
    printBanner("BUS MANAGEMENT - INSERT A RECORD");

    Bus b;
    memset(&b, 0, sizeof(Bus));

    printf("Enter Bus Details:\n");
    printf("--------------------------------------------------------------------------------\n");

    while (1) {
        b.b_id = getInt(" 1. Enter Bus ID (b_id, e.g. 101): ", 1, 99999);
        if (busExists(b.b_id)) {
            printf(" [!] Error: Bus with ID %d already exists. Enter a unique ID.\n", b.b_id);
        } else {
            break;
        }
    }

    getString(" 2. Enter Bus Name (b_name, max 20 chars): ", b.b_name, 20, 0);
    getString(" 3. Enter Starting Point (b_from, max 20 chars): ", b.b_from, 20, 0);
    getString(" 4. Enter Destination (b_to, max 20 chars): ", b.b_to, 20, 0);
    b.b_seats = getInt(" 5. Enter Number of Seats (b_seats, 1-100): ", 1, 100);
    b.b_fare  = getInt(" 6. Enter Bus Fare (b_fare, in Rs): ", 1, 100000);

    FILE *fp = fopen(BUS_FILE, "ab");
    if (!fp) {
        printf("\n [!] Error opening file %s for writing!\n", BUS_FILE);
        pauseScreen();
        return 0;
    }

    if (fwrite(&b, sizeof(Bus), 1, fp) == 1) {
        fclose(fp);
        printf("\n [OK] Bus record (ID: %d - %s) successfully inserted into %s!\n", b.b_id, b.b_name, BUS_FILE);
    } else {
        fclose(fp);
        printf("\n [!] Error writing record to %s!\n", BUS_FILE);
    }

    pauseScreen();
    return 1;
}

int editBus(void) {
    clearScreen();
    printBanner("BUS MANAGEMENT - EDIT A RECORD");

    FILE *fp = fopen(BUS_FILE, "rb");
    if (!fp) {
        printf(" [!] No bus records found in %s.\n", BUS_FILE);
        pauseScreen();
        return 0;
    }

    int target_id = getInt("Enter Bus ID to edit: ", 1, 99999);
    Bus b;
    int found = 0;

    FILE *temp = fopen("bus_temp.dat", "wb");
    if (!temp) {
        printf(" [!] Error creating temporary file.\n");
        fclose(fp);
        pauseScreen();
        return 0;
    }

    while (fread(&b, sizeof(Bus), 1, fp) == 1) {
        if (b.b_id == target_id) {
            found = 1;
            printf("\n--- Current Bus Details (ID: %d) ---\n", b.b_id);
            printf(" Name        : %s\n", b.b_name);
            printf(" From        : %s\n", b.b_from);
            printf(" To          : %s\n", b.b_to);
            printf(" Total Seats : %d\n", b.b_seats);
            printf(" Fare (Rs.)  : %d\n", b.b_fare);
            printf("------------------------------------\n");
            printf("Enter updated details (press Enter to keep existing):\n\n");

            char tempStr[64];
            getString(" New Name (press Enter to keep current): ", tempStr, 20, 1);
            if (strlen(tempStr) > 0) {
                strncpy(b.b_name, tempStr, 20);
                b.b_name[20] = '\0';
            }

            getString(" New Starting Point (b_from): ", tempStr, 20, 1);
            if (strlen(tempStr) > 0) {
                strncpy(b.b_from, tempStr, 20);
                b.b_from[20] = '\0';
            }

            getString(" New Destination (b_to): ", tempStr, 20, 1);
            if (strlen(tempStr) > 0) {
                strncpy(b.b_to, tempStr, 20);
                b.b_to[20] = '\0';
            }

            printf(" Update total seats? Current: %d\n", b.b_seats);
            if (confirmAction(" Change seats? (y/n): ")) {
                b.b_seats = getInt(" New Total Seats (1-100): ", 1, 100);
            }

            printf(" Update fare? Current: Rs. %d\n", b.b_fare);
            if (confirmAction(" Change fare? (y/n): ")) {
                b.b_fare = getInt(" New Fare (Rs.): ", 1, 100000);
            }
        }
        fwrite(&b, sizeof(Bus), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(BUS_FILE);
        rename("bus_temp.dat", BUS_FILE);
        printf("\n [OK] Bus record (ID: %d) successfully updated!\n", target_id);
    } else {
        remove("bus_temp.dat");
        printf("\n [!] Bus ID %d was not found in %s.\n", target_id, BUS_FILE);
    }

    pauseScreen();
    return found;
}

int deleteBus(void) {
    clearScreen();
    printBanner("BUS MANAGEMENT - DELETE A RECORD");

    FILE *fp = fopen(BUS_FILE, "rb");
    if (!fp) {
        printf(" [!] No bus records found in %s.\n", BUS_FILE);
        pauseScreen();
        return 0;
    }

    int target_id = getInt("Enter Bus ID to delete: ", 1, 99999);
    Bus b;
    int found = 0;

    FILE *temp = fopen("bus_temp.dat", "wb");
    if (!temp) {
        printf(" [!] Error creating temporary file.\n");
        fclose(fp);
        pauseScreen();
        return 0;
    }

    while (fread(&b, sizeof(Bus), 1, fp) == 1) {
        if (b.b_id == target_id) {
            found = 1;
            printf("\n--- Bus Found for Deletion ---\n");
            printf(" Bus ID      : %d\n", b.b_id);
            printf(" Name        : %s\n", b.b_name);
            printf(" Route       : %s -> %s\n", b.b_from, b.b_to);
            printf(" Seats       : %d\n", b.b_seats);
            printf(" Fare        : Rs. %d\n", b.b_fare);
            printf("-------------------------------\n");

            if (confirmAction(" Are you sure you want to permanently delete this bus record? (y/n): ")) {
                printf(" Record marked for deletion.\n");
                continue;
            } else {
                printf(" Deletion cancelled by user.\n");
                fwrite(&b, sizeof(Bus), 1, temp);
                continue;
            }
        }
        fwrite(&b, sizeof(Bus), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(BUS_FILE);
        rename("bus_temp.dat", BUS_FILE);
        printf("\n [OK] Deletion operation completed.\n");
    } else {
        remove("bus_temp.dat");
        printf("\n [!] Bus ID %d was not found.\n", target_id);
    }

    pauseScreen();
    return found;
}

void viewAllBuses(void) {
    clearScreen();
    printBanner("BUS MANAGEMENT - VIEW ALL RECORDS");

    FILE *fp = fopen(BUS_FILE, "rb");
    if (!fp) {
        printf(" [!] No bus records found or file %s cannot be opened.\n", BUS_FILE);
        pauseScreen();
        return;
    }

    Bus b;
    int count = 0;

    printf("+--------+----------------------+----------------------+----------------------+-------+----------+\n");
    printf("| Bus ID | Bus Name             | From                 | To                   | Seats | Fare(Rs) |\n");
    printf("+--------+----------------------+----------------------+----------------------+-------+----------+\n");

    while (fread(&b, sizeof(Bus), 1, fp) == 1) {
        printf("| %-6d | %-20s | %-20s | %-20s | %-5d | %-8d |\n",
               b.b_id, b.b_name, b.b_from, b.b_to, b.b_seats, b.b_fare);
        count++;
    }

    printf("+--------+----------------------+----------------------+----------------------+-------+----------+\n");
    printf(" Total Buses: %d\n", count);
    fclose(fp);
    pauseScreen();
}

void viewBusById(void) {
    clearScreen();
    printBanner("BUS MANAGEMENT - VIEW BUS BY ID");

    int id = getInt("Enter Bus ID to view: ", 1, 99999);
    Bus b;

    if (findBus(id, &b)) {
        printf("\n=======================================================\n");
        printf("                   BUS DETAILS\n");
        printf("=======================================================\n");
        printf(" Bus ID          : %d\n", b.b_id);
        printf(" Bus Name        : %s\n", b.b_name);
        printf(" Origin (From)   : %s\n", b.b_from);
        printf(" Destination(To) : %s\n", b.b_to);
        printf(" Total Seats     : %d\n", b.b_seats);
        printf(" Ticket Fare     : Rs. %d\n", b.b_fare);
        printf("=======================================================\n");
    } else {
        printf("\n [!] Bus ID %d not found in records.\n", id);
    }

    pauseScreen();
}

/* ============================================================================
 * PASSENGER FILE OPERATIONS
 * ============================================================================
 */
int passengerExists(int p_id) {
    FILE *fp = fopen(PASSENGER_FILE, "rb");
    if (!fp) return 0;
    Passenger p;
    while (fread(&p, sizeof(Passenger), 1, fp) == 1) {
        if (p.p_id == p_id) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int findPassenger(int p_id, Passenger *outPass) {
    FILE *fp = fopen(PASSENGER_FILE, "rb");
    if (!fp) return 0;
    Passenger p;
    while (fread(&p, sizeof(Passenger), 1, fp) == 1) {
        if (p.p_id == p_id) {
            if (outPass) *outPass = p;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int getPassengerCount(void) {
    FILE *fp = fopen(PASSENGER_FILE, "rb");
    if (!fp) return 0;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return (int)(size / sizeof(Passenger));
}

int insertPassenger(void) {
    clearScreen();
    printBanner("PASSENGER MANAGEMENT - INSERT A RECORD");

    Passenger p;
    memset(&p, 0, sizeof(Passenger));

    printf("Enter Passenger Details:\n");
    printf("--------------------------------------------------------------------------------\n");

    while (1) {
        p.p_id = getInt(" 1. Enter Passenger ID (p_id, e.g. 201): ", 1, 99999);
        if (passengerExists(p.p_id)) {
            printf(" [!] Error: Passenger with ID %d already exists. Enter a unique ID.\n", p.p_id);
        } else {
            break;
        }
    }

    getString(" 2. Enter Passenger Name (p_name, max 20 chars): ", p.p_name, 20, 0);
    getString(" 3. Enter Passenger Address (p_addr, max 40 chars): ", p.p_addr, 40, 0);

    FILE *fp = fopen(PASSENGER_FILE, "ab");
    if (!fp) {
        printf("\n [!] Error opening file %s for writing!\n", PASSENGER_FILE);
        pauseScreen();
        return 0;
    }

    if (fwrite(&p, sizeof(Passenger), 1, fp) == 1) {
        fclose(fp);
        printf("\n [OK] Passenger record (ID: %d - %s) successfully inserted into %s!\n", p.p_id, p.p_name, PASSENGER_FILE);
    } else {
        fclose(fp);
        printf("\n [!] Error writing record to %s!\n", PASSENGER_FILE);
    }

    pauseScreen();
    return 1;
}

int editPassenger(void) {
    clearScreen();
    printBanner("PASSENGER MANAGEMENT - EDIT A RECORD");

    FILE *fp = fopen(PASSENGER_FILE, "rb");
    if (!fp) {
        printf(" [!] No passenger records found in %s.\n", PASSENGER_FILE);
        pauseScreen();
        return 0;
    }

    int target_id = getInt("Enter Passenger ID to edit: ", 1, 99999);
    Passenger p;
    int found = 0;

    FILE *temp = fopen("passenger_temp.dat", "wb");
    if (!temp) {
        printf(" [!] Error creating temporary file.\n");
        fclose(fp);
        pauseScreen();
        return 0;
    }

    while (fread(&p, sizeof(Passenger), 1, fp) == 1) {
        if (p.p_id == target_id) {
            found = 1;
            printf("\n--- Current Passenger Details (ID: %d) ---\n", p.p_id);
            printf(" Name    : %s\n", p.p_name);
            printf(" Address : %s\n", p.p_addr);
            printf("------------------------------------------\n");
            printf("Enter updated details (press Enter to keep existing):\n\n");

            char tempStr[128];
            getString(" New Name (press Enter to keep current): ", tempStr, 20, 1);
            if (strlen(tempStr) > 0) {
                strncpy(p.p_name, tempStr, 20);
                p.p_name[20] = '\0';
            }

            getString(" New Address (press Enter to keep current): ", tempStr, 40, 1);
            if (strlen(tempStr) > 0) {
                strncpy(p.p_addr, tempStr, 40);
                p.p_addr[40] = '\0';
            }
        }
        fwrite(&p, sizeof(Passenger), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(PASSENGER_FILE);
        rename("passenger_temp.dat", PASSENGER_FILE);
        printf("\n [OK] Passenger record (ID: %d) successfully updated!\n", target_id);
    } else {
        remove("passenger_temp.dat");
        printf("\n [!] Passenger ID %d was not found in %s.\n", target_id, PASSENGER_FILE);
    }

    pauseScreen();
    return found;
}

int deletePassenger(void) {
    clearScreen();
    printBanner("PASSENGER MANAGEMENT - DELETE A RECORD");

    FILE *fp = fopen(PASSENGER_FILE, "rb");
    if (!fp) {
        printf(" [!] No passenger records found in %s.\n", PASSENGER_FILE);
        pauseScreen();
        return 0;
    }

    int target_id = getInt("Enter Passenger ID to delete: ", 1, 99999);
    Passenger p;
    int found = 0;

    FILE *temp = fopen("passenger_temp.dat", "wb");
    if (!temp) {
        printf(" [!] Error creating temporary file.\n");
        fclose(fp);
        pauseScreen();
        return 0;
    }

    while (fread(&p, sizeof(Passenger), 1, fp) == 1) {
        if (p.p_id == target_id) {
            found = 1;
            printf("\n--- Passenger Found for Deletion ---\n");
            printf(" Passenger ID : %d\n", p.p_id);
            printf(" Name         : %s\n", p.p_name);
            printf(" Address      : %s\n", p.p_addr);
            printf("------------------------------------\n");

            if (confirmAction(" Are you sure you want to permanently delete this passenger? (y/n): ")) {
                printf(" Record marked for deletion.\n");
                continue;
            } else {
                printf(" Deletion cancelled by user.\n");
                fwrite(&p, sizeof(Passenger), 1, temp);
                continue;
            }
        }
        fwrite(&p, sizeof(Passenger), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(PASSENGER_FILE);
        rename("passenger_temp.dat", PASSENGER_FILE);
        printf("\n [OK] Passenger record deletion completed.\n");
    } else {
        remove("passenger_temp.dat");
        printf("\n [!] Passenger ID %d was not found.\n", target_id);
    }

    pauseScreen();
    return found;
}

void viewAllPassengers(void) {
    clearScreen();
    printBanner("PASSENGER MANAGEMENT - VIEW ALL RECORDS");

    FILE *fp = fopen(PASSENGER_FILE, "rb");
    if (!fp) {
        printf(" [!] No passenger records found or file %s cannot be opened.\n", PASSENGER_FILE);
        pauseScreen();
        return;
    }

    Passenger p;
    int count = 0;

    printf("+--------------+----------------------+------------------------------------------+\n");
    printf("| Passenger ID | Passenger Name       | Address                                  |\n");
    printf("+--------------+----------------------+------------------------------------------+\n");

    while (fread(&p, sizeof(Passenger), 1, fp) == 1) {
        printf("| %-12d | %-20s | %-40s |\n",
               p.p_id, p.p_name, p.p_addr);
        count++;
    }

    printf("+--------------+----------------------+------------------------------------------+\n");
    printf(" Total Passengers: %d\n", count);
    fclose(fp);
    pauseScreen();
}

void viewPassengerById(void) {
    clearScreen();
    printBanner("PASSENGER MANAGEMENT - VIEW PASSENGER BY ID");

    int id = getInt("Enter Passenger ID to view: ", 1, 99999);
    Passenger p;

    if (findPassenger(id, &p)) {
        printf("\n=======================================================\n");
        printf("                 PASSENGER DETAILS\n");
        printf("=======================================================\n");
        printf(" Passenger ID : %d\n", p.p_id);
        printf(" Name         : %s\n", p.p_name);
        printf(" Address      : %s\n", p.p_addr);
        printf("=======================================================\n");
    } else {
        printf("\n [!] Passenger ID %d not found in records.\n", id);
    }

    pauseScreen();
}

/* ============================================================================
 * BUS RESERVATION FILE OPERATIONS
 * ============================================================================
 */
int reservationExists(int r_id) {
    FILE *fp = fopen(RESERVATION_FILE, "rb");
    if (!fp) return 0;
    Reservation r;
    while (fread(&r, sizeof(Reservation), 1, fp) == 1) {
        if (r.r_id == r_id) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int findReservation(int r_id, Reservation *outRes) {
    FILE *fp = fopen(RESERVATION_FILE, "rb");
    if (!fp) return 0;
    Reservation r;
    while (fread(&r, sizeof(Reservation), 1, fp) == 1) {
        if (r.r_id == r_id) {
            if (outRes) *outRes = r;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int getReservationCount(void) {
    FILE *fp = fopen(RESERVATION_FILE, "rb");
    if (!fp) return 0;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return (int)(size / sizeof(Reservation));
}

int getBookedSeats(int b_id, const char *r_date, int exclude_r_id) {
    FILE *fp = fopen(RESERVATION_FILE, "rb");
    if (!fp) return 0;
    Reservation r;
    int booked = 0;
    while (fread(&r, sizeof(Reservation), 1, fp) == 1) {
        if (r.b_id == b_id && strcmp(r.r_date, r_date) == 0 && r.r_id != exclude_r_id) {
            booked += r.r_seats;
        }
    }
    fclose(fp);
    return booked;
}

int getAvailableSeats(int b_id, const char *r_date, int exclude_r_id) {
    Bus b;
    if (!findBus(b_id, &b)) return -1;
    int booked = getBookedSeats(b_id, r_date, exclude_r_id);
    int avail = b.b_seats - booked;
    return (avail > 0) ? avail : 0;
}

void printTicket(int r_id) {
    Reservation r;
    if (!findReservation(r_id, &r)) {
        printf("\n [!] Reservation ID %d not found.\n", r_id);
        return;
    }

    Bus b;
    int busFound = findBus(r.b_id, &b);

    Passenger p;
    int passFound = 0;
    int pid_num = 0;
    if (sscanf(r.p_id, "%d", &pid_num) == 1) {
        passFound = findPassenger(pid_num, &p);
    }

    printf("\n");
    printf("********************************************************************************\n");
    printf("                      MACFAST BUS RESERVATION TICKET                            \n");
    printf("          MAR ATHANASIOS COLLEGE FOR ADVANCED STUDIES TIRUVALLA                 \n");
    printf("********************************************************************************\n");
    printf("  Reservation ID : %-10d                Booking Status : CONFIRMED\n", r.r_id);
    printf("  Journey Date   : %-15s             Trip Route     : %s to %s\n",
           r.r_date, busFound ? b.b_from : "N/A", busFound ? b.b_to : "N/A");
    printf("--------------------------------------------------------------------------------\n");
    printf("  [ BUS DETAILS ]\n");
    printf("  Bus ID         : %-10d                Bus Name       : %s\n",
           r.b_id, busFound ? b.b_name : "Unknown Bus");
    printf("  Origin (From)  : %-20s         Destination(To): %s\n",
           busFound ? b.b_from : "N/A", busFound ? b.b_to : "N/A");
    printf("  Fare per Seat  : Rs. %-10d             Total Seats Bus: %d\n",
           busFound ? b.b_fare : 0, busFound ? b.b_seats : 0);
    printf("--------------------------------------------------------------------------------\n");
    printf("  [ PASSENGER DETAILS ]\n");
    printf("  Passenger ID   : %-25s\n", r.p_id);
    if (passFound) {
        printf("  Passenger Name : %-20s         Address        : %s\n",
               p.p_name, p.p_addr);
    } else {
        printf("  Passenger Record: Not linked in Passenger file\n");
    }
    printf("--------------------------------------------------------------------------------\n");
    printf("  [ FARE & SEAT DETAILS ]\n");
    printf("  Seats Reserved : %-10d\n", r.r_seats);
    int totalFare = busFound ? (r.r_seats * b.b_fare) : 0;
    printf("  Total Fare     : Rs. %-10d\n", totalFare);
    if (busFound) {
        int availRemaining = getAvailableSeats(r.b_id, r.r_date, 0);
        printf("  Available Seats Remaining on Bus: %d\n", availRemaining);
    }
    printf("********************************************************************************\n");
    printf("        Wishing you a safe and pleasant journey! - MACFAST MCA GROUP 2         \n");
    printf("********************************************************************************\n\n");
}

int insertReservation(void) {
    clearScreen();
    printBanner("BUS RESERVATION - INSERT A RECORD (BOOK TICKET)");

    if (getBusCount() == 0) {
        printf(" [!] No buses available in %s. Please insert buses first!\n", BUS_FILE);
        pauseScreen();
        return 0;
    }

    Reservation r;
    memset(&r, 0, sizeof(Reservation));

    printf("Available Buses Summary:\n");
    FILE *fbus = fopen(BUS_FILE, "rb");
    if (fbus) {
        Bus tempB;
        printf("   ID | Bus Name             | Route                | Fare (Rs)\n");
        printf("  ----+----------------------+----------------------+----------\n");
        while (fread(&tempB, sizeof(Bus), 1, fbus) == 1) {
            char route[64];
            snprintf(route, sizeof(route), "%s -> %s", tempB.b_from, tempB.b_to);
            printf("  %-3d | %-20s | %-20s | %-8d\n", tempB.b_id, tempB.b_name, route, tempB.b_fare);
        }
        fclose(fbus);
    }
    printf("--------------------------------------------------------------------------------\n");

    while (1) {
        r.r_id = getInt(" 1. Enter Reservation ID (r_id, e.g. 1001): ", 1, 999999);
        if (reservationExists(r.r_id)) {
            printf(" [!] Error: Reservation ID %d already exists. Enter a unique ID.\n", r.r_id);
        } else {
            break;
        }
    }

    Bus selectedBus;
    while (1) {
        r.b_id = getInt(" 2. Enter Bus ID (b_id): ", 1, 99999);
        if (!findBus(r.b_id, &selectedBus)) {
            printf(" [!] Error: Bus ID %d does not exist. Choose from the available buses above.\n", r.b_id);
        } else {
            break;
        }
    }

    printf(" Selected Bus: %s (%s to %s) | Capacity: %d seats | Fare: Rs. %d\n",
           selectedBus.b_name, selectedBus.b_from, selectedBus.b_to, selectedBus.b_seats, selectedBus.b_fare);

    getDate(" 3. Enter Journey Date (r_date)", r.r_date, sizeof(r.r_date));

    int avail = getAvailableSeats(r.b_id, r.r_date, 0);
    printf("\n Checking Availability for Bus %d on %s...\n", r.b_id, r.r_date);
    printf(" Total Capacity   : %d seats\n", selectedBus.b_seats);
    printf(" Already Booked   : %d seats\n", selectedBus.b_seats - avail);
    printf(" Seats Available  : %d seats\n\n", avail);

    if (avail <= 0) {
        printf(" [!] Sorry, no seats available on Bus %d for date %s.\n", r.b_id, r.r_date);
        pauseScreen();
        return 0;
    }

    getString(" 4. Enter Passenger ID (p_id, e.g. 201 or P101): ", r.p_id, 25, 0);

    int pid_num = 0;
    Passenger p;
    if (sscanf(r.p_id, "%d", &pid_num) == 1) {
        if (findPassenger(pid_num, &p)) {
            printf(" -> Registered Passenger Found: %s, Address: %s\n", p.p_name, p.p_addr);
        } else {
            printf(" -> Passenger ID %d is not yet registered in %s.\n", pid_num, PASSENGER_FILE);
            if (confirmAction(" Would you like to register this passenger profile now? (y/n): ")) {
                Passenger newP;
                newP.p_id = pid_num;
                getString("    Enter Passenger Name (max 20 chars): ", newP.p_name, 20, 0);
                getString("    Enter Passenger Address (max 40 chars): ", newP.p_addr, 40, 0);
                FILE *fpPass = fopen(PASSENGER_FILE, "ab");
                if (fpPass) {
                    fwrite(&newP, sizeof(Passenger), 1, fpPass);
                    fclose(fpPass);
                    printf("    [OK] Passenger profile saved to %s!\n", PASSENGER_FILE);
                }
            }
        }
    }

    char prompt[128];
    snprintf(prompt, sizeof(prompt), " 5. Enter Number of Seats to Reserve (1-%d): ", avail);
    r.r_seats = getInt(prompt, 1, avail);

    int totalAmount = r.r_seats * selectedBus.b_fare;
    printf("\n--- Booking Summary ---\n");
    printf(" Bus           : %s (ID: %d)\n", selectedBus.b_name, r.b_id);
    printf(" Route         : %s -> %s\n", selectedBus.b_from, selectedBus.b_to);
    printf(" Date          : %s\n", r.r_date);
    printf(" Passenger ID  : %s\n", r.p_id);
    printf(" Seats Booked  : %d\n", r.r_seats);
    printf(" Total Fare    : Rs. %d\n", totalAmount);
    printf("-----------------------\n");

    if (!confirmAction(" Confirm this reservation? (y/n): ")) {
        printf(" [!] Reservation cancelled.\n");
        pauseScreen();
        return 0;
    }

    FILE *fp = fopen(RESERVATION_FILE, "ab");
    if (!fp) {
        printf(" [!] Error opening %s for writing!\n", RESERVATION_FILE);
        pauseScreen();
        return 0;
    }

    if (fwrite(&r, sizeof(Reservation), 1, fp) == 1) {
        fclose(fp);
        printf("\n [OK] Reservation successfully created!\n");
        printTicket(r.r_id);
    } else {
        fclose(fp);
        printf("\n [!] Failed to write reservation record.\n");
    }

    pauseScreen();
    return 1;
}

int editReservation(void) {
    clearScreen();
    printBanner("BUS RESERVATION - EDIT A RECORD");

    FILE *fp = fopen(RESERVATION_FILE, "rb");
    if (!fp) {
        printf(" [!] No reservation records found in %s.\n", RESERVATION_FILE);
        pauseScreen();
        return 0;
    }

    int target_id = getInt("Enter Reservation ID to edit: ", 1, 999999);
    Reservation r;
    int found = 0;

    FILE *temp = fopen("reservation_temp.dat", "wb");
    if (!temp) {
        printf(" [!] Error creating temporary file.\n");
        fclose(fp);
        pauseScreen();
        return 0;
    }

    while (fread(&r, sizeof(Reservation), 1, fp) == 1) {
        if (r.r_id == target_id) {
            found = 1;
            printf("\n--- Current Reservation Details (ID: %d) ---\n", r.r_id);
            printf(" Bus ID         : %d\n", r.b_id);
            printf(" Passenger ID   : %s\n", r.p_id);
            printf(" Journey Date   : %s\n", r.r_date);
            printf(" Seats Booked   : %d\n", r.r_seats);
            printf("--------------------------------------------\n");

            printf("\nOptions to Edit:\n");
            printf(" 1. Change Journey Date\n");
            printf(" 2. Change Number of Seats\n");
            printf(" 3. Change Passenger ID\n");
            printf(" 4. Keep All / Done\n");
            int choice = getInt(" Choose option (1-4): ", 1, 4);

            if (choice == 1) {
                char newDate[12];
                getDate(" Enter new Journey Date", newDate, sizeof(newDate));
                int availOnNewDate = getAvailableSeats(r.b_id, newDate, r.r_id);
                if (availOnNewDate < r.r_seats) {
                    printf(" [!] Not enough seats on %s (available: %d, needed: %d). Date unchanged.\n",
                           newDate, availOnNewDate, r.r_seats);
                } else {
                    strncpy(r.r_date, newDate, sizeof(r.r_date));
                    printf(" [OK] Date updated to %s.\n", r.r_date);
                }
            } else if (choice == 2) {
                int avail = getAvailableSeats(r.b_id, r.r_date, r.r_id);
                printf(" Seats available on %s for Bus %d: %d\n", r.r_date, r.b_id, avail);
                if (avail <= 0) {
                    printf(" [!] No other seats available on this bus date.\n");
                } else {
                    char prompt[128];
                    snprintf(prompt, sizeof(prompt), " Enter new seat count (1-%d): ", avail);
                    r.r_seats = getInt(prompt, 1, avail);
                    printf(" [OK] Seats updated to %d.\n", r.r_seats);
                }
            } else if (choice == 3) {
                getString(" Enter new Passenger ID (p_id, max 25 chars): ", r.p_id, 25, 0);
                printf(" [OK] Passenger ID updated to %s.\n", r.p_id);
            }
        }
        fwrite(&r, sizeof(Reservation), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(RESERVATION_FILE);
        rename("reservation_temp.dat", RESERVATION_FILE);
        printf("\n [OK] Reservation record (ID: %d) updated successfully!\n", target_id);
        printTicket(target_id);
    } else {
        remove("reservation_temp.dat");
        printf("\n [!] Reservation ID %d was not found in %s.\n", target_id, RESERVATION_FILE);
    }

    pauseScreen();
    return found;
}

int deleteReservation(void) {
    clearScreen();
    printBanner("BUS RESERVATION - DELETE / CANCEL A RECORD");

    FILE *fp = fopen(RESERVATION_FILE, "rb");
    if (!fp) {
        printf(" [!] No reservation records found in %s.\n", RESERVATION_FILE);
        pauseScreen();
        return 0;
    }

    int target_id = getInt("Enter Reservation ID to cancel/delete: ", 1, 999999);
    Reservation r;
    int found = 0;

    FILE *temp = fopen("reservation_temp.dat", "wb");
    if (!temp) {
        printf(" [!] Error creating temporary file.\n");
        fclose(fp);
        pauseScreen();
        return 0;
    }

    while (fread(&r, sizeof(Reservation), 1, fp) == 1) {
        if (r.r_id == target_id) {
            found = 1;
            printf("\n--- Reservation Found for Cancellation ---\n");
            printf(" Reservation ID : %d\n", r.r_id);
            printf(" Bus ID         : %d\n", r.b_id);
            printf(" Passenger ID   : %s\n", r.p_id);
            printf(" Date           : %s\n", r.r_date);
            printf(" Seats Booked   : %d\n", r.r_seats);
            printf("------------------------------------------\n");

            if (confirmAction(" Are you sure you want to cancel this reservation? (y/n): ")) {
                printf(" Reservation cancelled. Seats are now released back to the bus.\n");
                continue;
            } else {
                printf(" Cancellation aborted.\n");
                fwrite(&r, sizeof(Reservation), 1, temp);
                continue;
            }
        }
        fwrite(&r, sizeof(Reservation), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(RESERVATION_FILE);
        rename("reservation_temp.dat", RESERVATION_FILE);
        printf("\n [OK] Reservation cancellation completed.\n");
    } else {
        remove("reservation_temp.dat");
        printf("\n [!] Reservation ID %d was not found.\n", target_id);
    }

    pauseScreen();
    return found;
}

void viewAllReservations(void) {
    clearScreen();
    printBanner("BUS RESERVATION - VIEW ALL RECORDS");

    FILE *fp = fopen(RESERVATION_FILE, "rb");
    if (!fp) {
        printf(" [!] No reservation records found or file %s cannot be opened.\n", RESERVATION_FILE);
        pauseScreen();
        return;
    }

    Reservation r;
    int count = 0;

    printf("+--------+--------+---------------------------+------------+-------+-------------+\n");
    printf("| Res ID | Bus ID | Passenger ID              | Date       | Seats | Total Fare  |\n");
    printf("+--------+--------+---------------------------+------------+-------+-------------+\n");

    while (fread(&r, sizeof(Reservation), 1, fp) == 1) {
        Bus b;
        int fare = 0;
        if (findBus(r.b_id, &b)) {
            fare = r.r_seats * b.b_fare;
        }
        printf("| %-6d | %-6d | %-25s | %-10s | %-5d | Rs. %-7d |\n",
               r.r_id, r.b_id, r.p_id, r.r_date, r.r_seats, fare);
        count++;
    }

    printf("+--------+--------+---------------------------+------------+-------+-------------+\n");
    printf(" Total Reservations: %d\n", count);
    fclose(fp);
    pauseScreen();
}

void viewReservationById(void) {
    clearScreen();
    printBanner("BUS RESERVATION - VIEW TICKET / RECORD BY ID");
    int id = getInt("Enter Reservation ID: ", 1, 999999);
    printTicket(id);
    pauseScreen();
}

void checkBusAvailabilityUI(void) {
    clearScreen();
    printBanner("CHECK BUS SEAT AVAILABILITY");

    if (getBusCount() == 0) {
        printf(" [!] No buses found in %s.\n", BUS_FILE);
        pauseScreen();
        return;
    }

    int b_id = getInt("Enter Bus ID to check: ", 1, 99999);
    Bus b;
    if (!findBus(b_id, &b)) {
        printf(" [!] Bus ID %d not found.\n", b_id);
        pauseScreen();
        return;
    }

    char date[12];
    getDate("Enter Journey Date", date, sizeof(date));

    int booked = getBookedSeats(b_id, date, 0);
    int avail = b.b_seats - booked;
    if (avail < 0) avail = 0;

    printf("\n=======================================================\n");
    printf("                  SEAT AVAILABILITY REPORT              \n");
    printf("=======================================================\n");
    printf(" Bus ID          : %d\n", b.b_id);
    printf(" Bus Name        : %s\n", b.b_name);
    printf(" Route           : %s -> %s\n", b.b_from, b.b_to);
    printf(" Journey Date    : %s\n", date);
    printf(" Fare per Seat   : Rs. %d\n", b.b_fare);
    printf(" Total Capacity  : %d seats\n", b.b_seats);
    printf(" Seats Booked    : %d seats\n", booked);
    printf(" Seats Available : %d seats\n", avail);
    printf(" Status          : %s\n", (avail > 0) ? "AVAILABLE" : "FULLY BOOKED");
    printf("=======================================================\n");

    pauseScreen();
}

void showSystemSummary(void) {
    clearScreen();
    printBanner("SYSTEM SUMMARY & FILE STATUS");

    int numBuses = getBusCount();
    int numPass = getPassengerCount();
    int numRes = getReservationCount();

    printf("  Project Metadata:\n");
    printf("  - College        : Mar Athanasios College for Advanced Studies Tiruvalla\n");
    printf("  - Department     : Department of Computer Applications\n");
    printf("  - Program & Batch: MCA 2024-2026 Batch\n");
    printf("  - Group Number   : Group 2\n");
    printf("  - Project Name   : Bus Reservation System in C\n");
    printf("  - Coordinator    : Ms. Kripa Thankachan\n\n");

    printf("  File Statistics:\n");
    printf("  - BUS FILE            (%s)         : %d records\n", BUS_FILE, numBuses);
    printf("  - PASSENGER FILE      (%s)   : %d records\n", PASSENGER_FILE, numPass);
    printf("  - BUS RESERVATION FILE(%s) : %d records\n", RESERVATION_FILE, numRes);
    printf("--------------------------------------------------------------------------------\n");

    pauseScreen();
}

void seedSampleData(void) {
    clearScreen();
    printBanner("LOAD SAMPLE DEMO DATA");

    printf(" This will add sample buses, passengers, and reservations to the database\n");
    printf(" for immediate testing and demonstration.\n\n");

    if (!confirmAction(" Proceed with loading sample demo records? (y/n): ")) {
        printf(" Operation cancelled.\n");
        pauseScreen();
        return;
    }

    Bus sampleBuses[] = {
        {101, "KSRTC Swift Super", "Tiruvalla", "Trivandrum", 40, 280},
        {102, "Garuda Maharaja",   "Tiruvalla", "Bangalore",  36, 1250},
        {103, "Minnal Express",    "Kottayam",  "Kozhikode",  45, 420},
        {104, "Low Floor AC",      "Ernakulam", "Tiruvalla",  35, 210}
    };
    int busCount = sizeof(sampleBuses) / sizeof(sampleBuses[0]);

    FILE *fbus = fopen(BUS_FILE, "ab");
    if (fbus) {
        for (int i = 0; i < busCount; i++) {
            if (!busExists(sampleBuses[i].b_id)) {
                fwrite(&sampleBuses[i], sizeof(Bus), 1, fbus);
            }
        }
        fclose(fbus);
    }

    Passenger samplePass[] = {
        {201, "Rahul Nair",      "College Road, Tiruvalla"},
        {202, "Ananya Thomas",   "Kottayam Town, Kerala"},
        {203, "Mathew Varghese", "MG Road, Ernakulam"}
    };
    int passCount = sizeof(samplePass) / sizeof(samplePass[0]);

    FILE *fpass = fopen(PASSENGER_FILE, "ab");
    if (fpass) {
        for (int i = 0; i < passCount; i++) {
            if (!passengerExists(samplePass[i].p_id)) {
                fwrite(&samplePass[i], sizeof(Passenger), 1, fpass);
            }
        }
        fclose(fpass);
    }

    Reservation sampleRes[] = {
        {1001, 101, "201", "2026-10-25", 2},
        {1002, 102, "202", "2026-10-26", 1},
        {1003, 101, "203", "2026-10-25", 3}
    };
    int resCount = sizeof(sampleRes) / sizeof(sampleRes[0]);

    FILE *fres = fopen(RESERVATION_FILE, "ab");
    if (fres) {
        for (int i = 0; i < resCount; i++) {
            if (!reservationExists(sampleRes[i].r_id)) {
                fwrite(&sampleRes[i], sizeof(Reservation), 1, fres);
            }
        }
        fclose(fres);
    }

    printf("\n [OK] Sample demo data loaded successfully!\n");
    printf("   - %d buses checked/seeded.\n", busCount);
    printf("   - %d passengers checked/seeded.\n", passCount);
    printf("   - %d reservations checked/seeded.\n", resCount);

    pauseScreen();
}
