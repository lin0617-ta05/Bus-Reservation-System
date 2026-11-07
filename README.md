# Bus Reservation System in C
**Mar Athanasios College for Advanced Studies Tiruvalla (MACFAST)**  
**Department of Computer Applications — MCA 2024-2026 Batch**  
**C Project | Group No: 2**  
**Faculty Coordinator:** Ms. Kripa Thankachan  

---

## 📌 Project Overview
The **Bus Reservation System** is a robust, menu-driven C application built strictly according to the college MCA project specifications. It utilizes binary file handling (`bus.dat`, `passenger.dat`, `reservation.dat`) for persistent data storage across sessions, supports all required CRUD (Create, Read, Update, Delete) operations, manages real-time seat availability, and generates formatted boarding pass tickets.

---

## 📋 Document Specifications Compliance

### 1. BUS FILE (`bus.dat`)
| No | Description | Attribute | C Type | Implementation Detail |
|:---|:---|:---|:---|:---|
| 1 | Bus ID | `b_id` | `Int` | Unique identifier (Primary Key) |
| 2 | Name | `b_name` | `Char[20]` | Stored as `char[21]` with `\0` |
| 3 | Bus From | `b_from` | `Char[20]` | Boarding location / origin |
| 4 | Bus To | `b_to` | `Char[20]` | Destination |
| 5 | Number of seats | `b_seats` | `Int` | Bus seating capacity |
| 6 | Bus Fare | `b_fare` | `int` | Fare per passenger seat in Rupees |

### 2. PASSENGER FILE (`passenger.dat`)
| No | Description | Attribute | C Type | Implementation Detail |
|:---|:---|:---|:---|:---|
| 1 | Passenger ID | `p_id` | `Int` | Unique Passenger ID |
| 2 | Passenger Name | `p_name` | `Char[20]` | Passenger full name |
| 3 | Passenger Address | `p_addr` | `Char[40]` | Contact / residential address |

### 3. BUS RESERVATION FILE (`reservation.dat`)
| No | Description | Attribute | C Type | Implementation Detail |
|:---|:---|:---|:---|:---|
| 1 | Reservation ID | `r_id` | `int` | Unique ticket / booking reference |
| - | Bus ID | `b_id` | `int` | References existing bus from `bus.dat` |
| 2 | Passenger ID | `p_id` | `Char[25]` | Matches document spec `Char[25]`, cross-referenced with `passenger.dat` |
| 3 | Date | `r_date` | `Char[10]` | Journey date format `YYYY-MM-DD` |
| 4 | Number of Seats Available / Reserved | `r_seats` | `int` | Number of seats booked with availability tracking |

---

## ⚙️ Operations Supported (Across All Files)
- **Insert a Record**: Add new buses, passengers, and reservations with validation and uniqueness checks.
- **Edit a Record**: Modify existing records with current value display and optional overwrite.
- **Delete a Record**: Remove records with confirmation prompts. Deleting a reservation automatically releases seats back to the bus.
- **View**:
  - View all records in clean formatted tables.
  - View individual record details by ID.
  - Generate full-screen formatted **Boarding Pass Ticket**.
  - Check real-time seat availability for any bus on any date.

---

## 📂 Project Structure
```
bus_reservation_system/
├── build.bat                  # One-click Windows build script
├── models.h                   # Structure definitions for Bus, Passenger, Reservation
├── utils.h & utils.c          # Console UI, safe string/integer input, date validation
├── bus.h & bus.c              # Bus file operations (CRUD)
├── passenger.h & passenger.c  # Passenger file operations (CRUD)
├── reservation.h & reservation.c # Reservation operations, availability & ticket generation
├── main.c                     # Menu-driven system driver and demo data seeder
├── bus_reservation_all_in_one.c # Standalone single-file version for lab submission
├── README.md                  # Complete documentation
└── *.dat                      # Binary data files (bus.dat, passenger.dat, reservation.dat)
```

---

## 🚀 How to Compile and Run

### Option 1: One-Click Build Script (Recommended on Windows)
Simply double-click `build.bat` or run:
```bat
build.bat
```
Then execute:
```bat
bus_reservation.exe
```

### Option 2: Command Line using GCC (Modular)
```bash
gcc -Wall -Wextra -std=c99 main.c bus.c passenger.c reservation.c utils.c -o bus_reservation.exe
./bus_reservation.exe
```

### Option 3: Command Line using GCC (Single All-in-One File)
```bash
gcc -Wall -Wextra -std=c99 bus_reservation_all_in_one.c -o bus_reservation.exe
./bus_reservation.exe
```

---

## 🎯 Quick Demonstration / Testing
1. Launch `bus_reservation.exe`.
2. Choose **Option 6 (LOAD SAMPLE DEMO DATA)** to immediately pre-populate sample buses, passengers, and bookings.
3. Choose **Option 1 -> 4** to view all buses.
4. Choose **Option 2 -> 4** to view all passengers.
5. Choose **Option 3 -> 4** to view all reservations.
6. Choose **Option 4** and enter reservation ID `1001` to view a sample ticket.
7. Choose **Option 5** to test live seat availability calculation.
8. Perform Insert, Edit, Delete on any menu to demonstrate full CRUD functionality.
