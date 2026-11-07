#include "bus.h"
#include "utils.h"

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
                continue; /* Skip writing this record to temp file */
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
