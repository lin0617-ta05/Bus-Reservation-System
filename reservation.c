#include "reservation.h"
#include "bus.h"
#include "passenger.h"
#include "utils.h"

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
    if (!findBus(b_id, &b)) {
        return -1;
    }
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

    /* Check if passenger info exists in passenger.dat */
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

    /* Passenger ID: char[25] according to document specification */
    getString(" 4. Enter Passenger ID (p_id, e.g. 201 or P101): ", r.p_id, 25, 0);

    /* Look up passenger in passenger.dat */
    int pid_num = 0;
    Passenger p;
    int passFound = 0;
    if (sscanf(r.p_id, "%d", &pid_num) == 1) {
        passFound = findPassenger(pid_num, &p);
        if (passFound) {
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

    /* Seats to reserve: r_seats */
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
                continue; /* Skip writing this record to temp file */
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
