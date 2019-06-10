#ifndef PINHIT_H_
#define PINHIT_H_

class PinHit {

    public:
        PinHit();
        PinHit(int key, int value);
        void SetKey(int pin);
        int GetKey() const;
        int GetValue() const;
        void IncrementHits();
        int CompareTo(const PinHit& other) const;
        bool operator==(const PinHit& other);

    private:

        int m_pin, m_hits;
};

#endif /* PinHit_H_ */
