#include <assert.h>
#include "PinHit.h"

/*
 * default constuctor
 * Initializes a PinHit with default values for PIN and frequency. By default
 * the pin number and number of hits should be -1,0.
 *
 * Pin hits are simply Key Value pairs where:
 * Key is the pin number
 * Value is the number of times that the pin number was seen in the dataset
 */
PinHit::PinHit() {
    m_pin = -1; // key
    m_hits = 0; // value
}

/*
 * constructor
 * Initializes a PinHit with the PIN and the number of hits (occurences) it has
 */
PinHit::PinHit(int key, int value) {
    assert(0 <= key && key <= 9999); // stop program if key is invalid

    m_pin = key;
    m_hits = value;
}

/*
 * Sets the value of the pin
 */
void PinHit::SetKey(int pin) {
    assert(0 <= pin && pin <= 9999);
    m_pin = pin;
}

/*
 * returns the pin number, used as a key in the key-value pair
 */
int PinHit::GetKey() const {
    return m_pin;
}

/*
 * returns the number of hits, used as a value in this key-value pair
 */
int PinHit::GetValue() const {
    return m_hits;
}

/*
 * Increments the number of hits this pin has encountered
 */
void PinHit::IncrementHits() {
    m_hits++;
}

/*
 * Compares the Value (hits) of this PinHit. Returns 0 if this is the same as
 * other, negative if this is less than other, or positive if this is greater
 * than other. Note, this does not compare the pin numbers in any way.
 */
int PinHit::CompareTo(const PinHit& other) const {
    if (m_hits == other.m_hits)
        return 0;
    if (m_hits < other.m_hits)
        return -1;
    return 1; // m_hits > other.m_hits
}

/*
 * Compare the Key (pins) against other for equality. If this pin is equivalent
 * to other’s pin, return true. Otherwise, return false.
 */
bool PinHit::operator==(const PinHit& other) {
    if (m_pin == other.m_pin)
        return true;
    return false;
}
