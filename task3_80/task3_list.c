#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task3_list.h"

// A registry to keep track of frequent fliers
typedef struct registry_t REGISTRY;
struct registry_t {
	char name[NAME_LEN];
	int count;
	struct registry_t *next;
};

static REGISTRY *registryHead = NULL;

static void registry_add(const char *name) {
	REGISTRY *registry = registryHead;
	while(registry) {
		if(strcmp(registry->name, name) == 0) {
			registry->count++;
			return;
		}
		registry = registry->next;
	}
	registry = malloc(sizeof *registry);
	strncpy(registry->name, name, NAME_LEN-1);
	registry->name[NAME_LEN-1] = 0;
	registry->count = 1;
	registry->next = registryHead;
	registryHead = registry;
}

static void registry_remove(const char *name) {
	REGISTRY **registryPassenger = &registryHead;

	while(*registryPassenger) {
		if(strcmp((*registryPassenger)->name, name) == 0) {
			(*registryPassenger)->count--;
			if((*registryPassenger)->count <= 0) {
				REGISTRY *toFree = *registryPassenger;
				*registryPassenger = toFree->next;
				free(toFree);
			}
			return;
		}
		registryPassenger = &(*registryPassenger)->next;
	}
}

// Adds a new flight to the end of the double-linked list
int addFlight(FLIGHT **head, const char *flightID, const char *destination, int seats, int departureTime) {
	FLIGHT *newFlight;
	FLIGHT *current;

	newFlight = (FLIGHT *)malloc(sizeof *newFlight);
	if(newFlight == NULL) {
		return -1;
	}

	strncpy(newFlight->flightID, flightID, ID_LEN-1);
	newFlight->flightID[ID_LEN-1] = 0;
	strncpy(newFlight->destination, destination, DEST_LEN-1);
	newFlight->destination[DEST_LEN-1] = 0;
	newFlight->seats = seats;
	newFlight->departureTime = departureTime;
	newFlight->passengerHead = NULL;
	newFlight->next = NULL;

	if(*head == NULL) {
		newFlight->prev = NULL;
		*head = newFlight;
	} else {
		current = *head;
		while(current->next != NULL) {
			current = current->next;
		}
		current->next = newFlight;
		newFlight->prev = current;
	}
	return 0;
}

// Adds a new passenger, sorted by seatNr and gets added to the passenger registry
int addPassenger(FLIGHT *head, const char *flightID, int seatNr, const char *name, int age) {
	FLIGHT *flight;
	PASSENGER *newPassenger;
	PASSENGER **passengerSort;
	flight = head;

	// Finds the flight by its ID
	while(flight && strcmp(flight->flightID, flightID)) {
		flight = flight->next;
	}
	if(!flight)
		return -1;

// Allocate and populate the new passenger node
	newPassenger = (PASSENGER *)malloc(sizeof *newPassenger);
	if(!newPassenger)
		return -1;

	newPassenger->seatNr = seatNr;
	strncpy(newPassenger->name, name, NAME_LEN-1);
	newPassenger->name[NAME_LEN-1] = 0;
	newPassenger->age = age;

// Inserts into a sorted single-linked passenger list, having passengerSort as the
// insertion point
	passengerSort = &flight->passengerHead;
	while(*passengerSort && (*passengerSort)->seatNr < seatNr) {
		passengerSort = &(*passengerSort)->next;
	}
	newPassenger->next = *passengerSort;
	*passengerSort = newPassenger;
	registry_add(newPassenger->name); // Updates passenger registry
	return 0;
}

// Prints out information about a flight (by its id) and each passenger on that flight
int printFlightInfo(FLIGHT *head, int itemNumber) {
	FLIGHT *flight = head;
	PASSENGER *passenger;
	int id = 1;

	while(flight && id < itemNumber) {
		flight = flight->next;
		id++;
	}
	if(!flight) {
		printf("No flight at index %d\n", itemNumber);
		return -1;
	}

	printf("Flight %d\n", id);
	printf(" ID: %s\n", flight->flightID);
	printf(" Destination: %s\n", flight->destination);
	printf(" Seats: %d\n", flight->seats);
	printf(" Departure Time: %04d\n", flight->departureTime);
	printf(" Passengers:\n");
	
	passenger = flight->passengerHead;
	while(passenger != NULL) {
		printf(" Seat %3d %s, age %d\n",
		passenger->seatNr, passenger->name, passenger->age);
		passenger = passenger->next;
	}
	return 0;
}

// Finds all flights going to a certain destination
int flightByDestination(FLIGHT *head, const char *destination) {
	FLIGHT *flight = head;
	int id = 1;

	while(flight) {
		if(strcmp(flight->destination, destination) == 0) {
			printf("Found at index %d\n", id);
			return id;
		}
		flight = flight->next;
		id++;
	}
	printf("Destination %s not found\n", destination);
	return -1;
}

// Unlinks and frees both the flight and its passengers, and removes the passengers from the
// passenger registry
int deleteFlight(FLIGHT **head, const char *flightID) {
	FLIGHT *flight = *head;
	PASSENGER *passenger;
	PASSENGER *temp;
	int count = 0;

// Locates the flight we want to delete (by its id)
	while(flight && strcmp(flight->flightID,flightID)) {
		flight = flight->next;
	}
	if(!flight) {
		printf("Flight ID %s not found\n", flightID);
		return -1;
	}

// Unlinks it from the double-linked flight list
	if(flight->prev) {
		flight->prev->next = flight->next;
	} else {
		*head = flight->next;
	}
	if(flight->next) {
		flight->next->prev = flight->prev;
	}

// Free each passenger nodes and updates the passenger registry
	passenger = flight->passengerHead, *temp;
	while(passenger) {
		temp = passenger->next;
		registry_remove(passenger->name);
		free(passenger);
		passenger = temp;
		count++;
	}
	printf("Deleted flight %s and its %d passengers\n", flightID, count);
	free(flight); // Free the flight node
	return 0;
}

// Unlinks a passenger from their old seatNr and reinserts them to their new seatNr,
// while maintaining the sort order
int changeSeat(FLIGHT *head, const char *flightID, int oldSeat, int newSeat) {
	FLIGHT *flight;
	PASSENGER **passengerSort;
	PASSENGER *passenger;
	flight = head;

	while(flight != NULL && strcmp(flight->flightID, flightID) != 0) {
		flight = flight->next;
	}
	if(flight == NULL) {
		printf("Flight %s not found\n", flightID);
		return -1;
	}

	passengerSort = &flight->passengerHead;
	while(*passengerSort != NULL && (*passengerSort)->seatNr != oldSeat) {
		passengerSort = &(*passengerSort)->next;
	}
	if(*passengerSort == NULL) {
		printf("Seat %d not found on flight %s\n", oldSeat, flightID);
		return -1;
	}

	passenger = *passengerSort;
	*passengerSort = passenger->next;
	passenger->seatNr = newSeat;
	passengerSort = &flight->passengerHead;
	while(*passengerSort != NULL && (*passengerSort)->seatNr < newSeat) {
		passengerSort = &(*passengerSort)->next;
	}
	passenger->next = *passengerSort;
	*passengerSort = passenger;
	printf("Moved %s from seat %d to seat %d on flight %s\n",
	passenger->name, oldSeat, newSeat, flightID);
	return 0;
}

// Finds all flights a passenger is booked on
int findPassengerFlights(FLIGHT *head, const char *name) {
	FLIGHT *flight;
	PASSENGER *passenger;
	flight = head;
	int found = 0;

	while(flight != NULL) {
		passenger = flight->passengerHead;
		while(passenger != NULL) {
			if(strcmp(passenger->name, name) == 0) {
				if(found == 0) {
					printf("%s is booked on:\n", name);
				}
				printf("%s > %s at %04d, seat %d\n",
				flight->flightID, flight->destination,
				flight->departureTime, passenger->seatNr);
				found++;
			}
			passenger = passenger->next;
		}
		flight = flight->next;
	}
	if(found == 0) {
		printf("no reservations found for %s\n", name);
		return -1;
	}
	return found;
}

// Finds all, if any, passengers that are booked on multiple flights
int findFrequentFliers(FLIGHT *head) {
	REGISTRY *registry = registryHead;
	int any = 0;

	while(registry) {
		if(registry->count > 1) {
			printf("Frequent flier: %s (%d flights)\n",
			registry->name, registry->count);
			any = 1;
		}
		registry = registry->next;
	}
	if(!any) {
		printf("no frequent fliers found\n");
		return -1;
	}
	return any;
}

// Frees every passengers and flights, also removing them from the passenger registry
void freeAllFlights(FLIGHT *head) {
	PASSENGER *passenger;
	FLIGHT *temp;
	REGISTRY *current;
	REGISTRY *next;

	while(head) {
		passenger = head->passengerHead;
		while(passenger) {
			PASSENGER *nextPassenger = passenger->next;
			free(passenger);
			passenger = nextPassenger;
		}
		temp = head;
		head = head->next;
		free(temp);
	}
	current = registryHead;
	while(current) {
		next = current->next;
		free(current);
		current = next;
	}
	registryHead = NULL;
}
