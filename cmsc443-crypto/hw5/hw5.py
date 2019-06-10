#!/usr/bin/env python3

def main():
    # test algo5_3 on exercise 5.3:
    ans = [0]*3
    ans[0] = algo5_3(101, 17)         # a) 17^-1 mod 101 = 6
    ans[1] = algo5_3(1234, 357)       # b) 357^-1 mod 1234 = 1075
    ans[2] = algo5_3(9987, 3125)      # c) 3125^-1 mod 9987 = 1844
    for i in range(3):
        print("5.3 " + chr(97 + i) + ") " + str(ans[i]))

    # test algo5_5:
    ans = algo5_5(72993, 432, 1001)   # (72993^432) mod 1001 = 911
    print("\n(72993^432) mod 1001 = " + str(ans))

# Algorithm 5.3 (Multiplicative inverse)
# see p. 168 in textbook
# a: modulo base (int)
# b: number to find the inverse of
def algo5_3(a, b):
    a0 = a
    b0 = b
    t0 = 0
    t = 1
    q = int(a0 // b0)          # floor
    r = a0 - q*b0
    while r > 0:
        temp = (t0 - q*t) % a
        t0 = t
        t = temp
        a0 = b0
        b0 = r
        q = int(a0/b0)
        r = a0 - q*b0
    if b0 != 1:
        return None            # b has no inverse modulo a
    return t

# Algorithm 5.5 (square and multiply)
# see p 177 in textbook
# computes: z = x^c mod n
def algo5_5(x, c, n):
    c = bin(c)[2::]  # convert c from a integer to binary (str of 1s and 0s)
    z = 1
    l = len(c)
    for i in range(0, l):
        z = (z**2) % n
        if int(c[i]) == 1:
            z = (z * x) % n
    return z

if __name__ == "__main__":
    main()
