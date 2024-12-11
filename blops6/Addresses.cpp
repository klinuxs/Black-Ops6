#include "Addresses.h"

uint64_t Addresses::base = 0;

Addresses::Addresses() { }

void Addresses::SetAddress(uint64_t Address) {
	base = Address;
}