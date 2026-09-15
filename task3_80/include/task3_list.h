#ifndef TASK3_LIST_H
#define TASK3_LIST_H

#define ID_LEN 16
#define DEST_LEN 32
#define NAME_LEN 32

// Forward declarations so that PASSENGER and FLIGHT can refer to each other
typedef struct passenger_t PASSENGER;
typedef struct flight_t FLIGHT;

// Single-linked list of passenger reservations
struct passenger_t {
	int seatNr;
	char name[NAME_LEN];
	int age;
	struct passenger_t *next;
};

// Double-linked list of departures
struct flight_t {
	char flightID[ID_LEN];
	char destination[DEST_LEN];
	int seats;
	int departureTime;
	struct flight_t *prev, *next;
	PASSENGER *passengerHead;
};

// Prototypes for the menu commands
int addFlight (FLIGHT **head, const char *flightID, const char *destination, int seats, int departureTime);

int addPassenger (FLIGHT *head, const char *flightID, int seatNr, const char *name, int age);

int printFlightInfo (FLIGHT *head, int itemNumber);

int flightByDestination (FLIGHT *head, const char *destination);

int deleteFlight (FLIGHT **head, const char *flightID);

int changeSeat (FLIGHT *head, const char *flightID, int oldSeat, int newSeat);

int findPassengerFlights (FLIGHT *head, const char *name);

int findFrequentFliers (FLIGHT *head);

void freeAllFlights (FLIGHT *head);

#endif // TASK3_LIST_H
