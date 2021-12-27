

extern int setBit(int val, int pos);
extern int getBit(int val, int pos);
extern int clearBit(int val, int pos);

int setBit(int val, int pos) {
    val |= 1<< pos;
    return val;
}

int getBit(int val, int pos) {
    int bit = (val >> pos) & 1;
    return bit;
}

int clearBit(int val, int pos) {
    val &= ~(1 << pos);
    return val;
}