#!/usr/bin/env python3
# Dan Engbert, CMSC443, Spring18
#
# this file implements functions for creating Elliptic curve (Secp256k1) keys
# and encrypting/decrypting strings with the keys using the
# simplified ECIES system from p.264 in "Cryptography Theory and Practice" ed3
import random
import math
import secrets

# global variables

def main():
    # Elliptic curve parameters for Secp256k1 (y^2 = x^3 + ax + b)
    p = 115792089237316195423570985008687907853269984665640564039457584007908834671663
    a, b = 0, 7
    Gx = 55066263022277343669578718895168534326250603453777594175500187360389116729240
    Gy = 32670510020758816978083085130507043184471273380659243275938904335757337482424
    E = (p, a, b, (Gx, Gy))  # P = (Gx, Gy)
    tests(E) # perform some tests that everything is working

    # create public and private keys
    # I tested my public/private key using http://www.christelbach.com/eccalculator.aspx
    #createKeys(E)                # create key files and write to file
    pubK, privK = readKeys()     # read key files from file

    # encrypt a string
    #k = secrets.randbelow(E[0]-1) + 1           # random int to use for encryption
    #data = str2int("hello world")
    #val = encrypt(data, k, pubK, E)
    #with open('test_cipher.txt','w') as f:
    #    f.write(str(val[0][0]))
    #    f.write('\n')
    #    f.write(str(val[0][1]))
    #    f.write('\n')
    #    f.write(str(val[1]))

    #with open('test_cipher.txt','r') as f:
    with open('engbert_cipher.txt','r') as f:
        kP_x = int(f.readline())
        kP_y = int(f.readline())
        z = int(f.readline())
    val = ((kP_x, kP_y), z)
    print("decrypting:")
    print(val)
    print("-----")
    data2 = int2str(decrypt(val, privK, E))
    print("decrypted to: '" + str(data2) + "'")

    # write result to file
    with open('engbert_text.txt','w') as f:
        f.write(str(data2) + "\n")

# perform some tests to ensure my functions are working correctly
def tests(E):
    # simple test that encryption/decryption works based on p.264-265 example in textbook
    E1 = (11, 1, 7, (2, 7))
    pubK, privK = (7, 2), 7
    k = 6
    val = encrypt(9, k, pubK, E1)
    val = decrypt(val, privK, E1)
    assert(9 == val)

    # tests from professor:
    R = addPoints(E[3], E[3], E)                # R = P + P
    assert(R == (89565891926547004231252920425935692360644145829622209833684329913297188986597,
 12158399299693830322967808612713398636155367887041628176798871954788371653930))

    S1 = addPoints(E[3], R, E)                   # S1 = P + R
    assert(S1 == (112711660439710606056748659173929673102114977341539408544630613555209775888121,
 25583027980570883691656905877401976406448868254816295069919888960541586679410))
    S2 = addPoints(R, E[3], E)                   # S2 = R + P
    assert(S1 == S2)


    k = 123456789
    T = doubleAndAdd(E[3], k, E)                # T = kP
    assert(T == (4051293998585674784991639592782214972820158391371785981004352359465450369227,
    88166831356626186178414913298033275054086243781277878360288998796587140930350))

    print("passed all tests")


# decrypt data with the given private key
# y = tuple (Y1, z) where:
#    Y1 = kP (coordinate tuple)
#    z = x_data * kQ.x (mod p)  (int)
# return: int
def decrypt(y, privK, E):
    Y1 = y[0]
    z = y[1]
    kQ = doubleAndAdd(Y1, privK, E)             # mY1 = kQ
    return (z * algo5_3(kQ[0], E[0])) % E[0]    # z * (kQ.x)^-1 mod p

# encrypt data with the given public key
# x_data: int to encrypt
# k:      random int to use (0 < k < p)
# pubK:   coordinate tuple (Q.x, Q.y)
def encrypt(x_data, k, pubK, E):
    Y1 = doubleAndAdd(E[3], k, E)               # kP
    kQ = doubleAndAdd(pubK, k, E)               # kQ
    z = (x_data * kQ[0]) % E[0]                 # x_data * kQ.x (mod p)
    return (Y1, z)

# reads (pubK, privK) from files
# return ((Q.x,Q.y), m)
def readKeys():
    pubK = []     # [Qx, Qy]
    with open('engbert_keys.txt','r') as f:
        for line in f:
            pubK += [int(line)]
    with open('engbert_keys_private.txt','r') as f:
        privK = int(f.readline())
    print("read keys from files.")
    return (tuple(pubK), privK)

# create public and private keys and write them to a file
# (see p. 266 in textbook)
# return: void
def createKeys(E):
    m = secrets.randbelow(E[0]-1) + 1           # random number 1 <= m <= p
    Q = doubleAndAdd(E[3], m, E)

    # write public key to file
    with open('engbert_keys.txt','w') as f:
        f.write(str(Q[0])) # x coordinate of Q
        f.write("\n")
        f.write(str(Q[1])) # y coordinate of Q ( we aren't using point-compress)

    # write private key to file
    with open('engbert_keys_private.txt','w') as f:
        f.write(str(m))
    print("wrote keys to files.")

# perform Q = m*P on the elliptical curve E
# modified version of algo5_5 (replaced squaring with doubling and adding with multiplying)
# (see p. 266 in textbook)
def doubleAndAdd(P, m, E):
    c = bin(m)[2::]             # convert c from a integer to binary (str of 1s and 0s)
    Z = (-1, -1)                # infinity point
    for i in range(len(c)):
        Z = addPoints(Z, Z, E)
        if int(c[i]) == 1:
            Z = addPoints(Z, P, E)
    return (Z[0] % E[0], Z[1] % E[0])

# add two points (P and Q) on the elliptical curve E
# (see p. 258 in textbook)
# P: (x1, y1)
# Q: (x2, y2)
def addPoints(P, Q, E):
    x1, y1 = P
    x2, y2 = Q
    O = (-1, -1)                                # identity element
    if P == O or Q == O:
        return Q if P == O else P

    if P[0] == Q[0] and Q[1] == -P[1]:
        return O
    if P == Q:
        lamda = ((3*x1**2 + E[1]) * algo5_3(2*y1, E[0])) % E[0]
    else:
        lamda = ((y2-y1) * algo5_3(x2-x1, E[0])) % E[0]
    x3 = (lamda**2 - x1 - x2) % E[0]
    y3 = (lamda * (x1 - x3) - y1) % E[0]
    return (x3, y3)

# Algorithm 5.3 (Multiplicative inverse)
# (see p. 168 in textbook)
# a:      modulo base (int)
# b:      number to find the inverse of
# return: int t^-1 mod a
def algo5_3(t, a):
    t = t % a             # make sure it's positive
    b = t
    a0 = a
    b0 = b
    t0 = 0
    t = 1
    q = int(a0 // b0)     # floor (must use integer division for accuracy with large ints)
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
        return None       # b has no inverse modulo a
    return t

# Algorithm 5.5 (square and multiply)
# (see p. 177 in textbook)
# x:      int
# c:      int
# n:      int
# return: int z = x^c mod n
def algo5_5(x, c, n):
    if c <= 0:
        return 1
    c = bin(c)[2::]       # convert c from a integer to binary (str of 1s and 0s)
    z = 1
    for i in range(len(c)):
        z = (z**2) % n
        if int(c[i]) == 1:
            z = (z * x) % n
    return z

# Convert a string to a big integer
# s:      str
# return: int
def str2int(s):
    s = s.encode('utf-8')
    return int(s.hex(), 16)

# Convert a big integer to a string
# i:      int
# return: str
def int2str(i):
    h = hex(i)
    if len(h) > 1 and h[0:2] == '0x':
        h = h[2:]
    if h[len(h)-1] == 'L':
        h = h[0:len(h)-1]
    if len(h) % 2 == 1:       # zero pad if necessary
        h = "0" + h
    return bytearray.fromhex(h).decode('utf-8')

if __name__ == "__main__":
    main()
