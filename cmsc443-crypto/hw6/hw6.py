#!/usr/bin/env python3
# this file implements functions for creating RSA keys
# and encrypting/decrypting strings with the keys
import random
import math

def main():
    # create RSA key files
    #createRSAKeys()

    # read keys from files
    pubK = []
    privK = []
    with open('engbert_keys.txt','r') as f:
        for line in f:
            pubK += [int(line)]
    with open('engbert_keys_private.txt','r') as f:
        for line in f:
            privK += [int(line)]

    # encrypt data and write it to a file
    #data = "this message is hidden"             # message to encrypt
    #with open('engbert_cipher.txt', 'w') as f:
    #    y = encrypt(pubK, str2int(data))
    #    f.write(str(y))                         # write the integer to the file

    # decrypt data in the file
    with open('engbert_cipher.txt', 'r') as f:
        y = int(f.readline())
        data2 = int2str(decrypt(privK, y))
    print("file contents decrypted to:\n" + data2)

    # write decrypted data to a file
    with open('engbert_text.txt', 'w') as f:
        f.write(data2)


# decrypt data with the given private key
# privK:  tuple (int p, intq, int a)
# y:      int data to be decrypted
# return: int
#
# example usage: data = int2str(decrypt(privK, y))
def decrypt(privK, y):
    p = privK[0]
    q = privK[1]
    a = privK[2]
    n = p * q
    return algo5_5(y, a, n)    # y^a mod n

# encrypt data with the given public key
# pubK:   tuple (int n, int b)
#      x: int data to be encrypted
# return: int
#
# example usage: y = encrypt(pubK, str2int("secret message"))
def encrypt(pubK, x):
    n = pubK[0]
    b = pubK[1]
    return algo5_5(x, b, n)    # x^b mod n

# create RSA public and private keys and write them to a file
# return: void
def createRSAKeys():
    while 1:
        # get two 512 bit primes
        p = getAPrime()
        q = getAPrime()
        phi = (p-1)*(q-1)
        n = p * q

        b = 2**16 + 1 # known to be prime
        # ensure that b and phi are coprime
        if math.gcd(b, phi) == 1:
            a = algo5_3(phi, b)   # a = the inverse of b mod phi
            assert (a*b) % ((p-1)*(q-1)) == 1
            break

    print("\npublic key:\n" + str((n,b)) + "\n\nprivate key:\n" + str((p, q, a)))
    # write keys to files
    keyFid = open('engbert_keys.txt','w');
    keyFid.write(str(n) + '\n' + str(b))
    keyFid.close()

    keyFid = open('engbert_keys_private.txt','w');
    keyFid.write(str(p) + '\n' + str(q) + '\n' + str(a))
    keyFid.close()

# finds and returns a 512 bit prime number
# (number is very likely to be prime based on 20 iterations of miller rabin algorithm)
# return: int
def getAPrime():
    isPrime = False
    while not isPrime:
        s = random.getrandbits(512)
        if s % 2 == 0:
            s += 1

        # test if s is prime using 20 iterations of the Miller-Rabin test
        for i in range(20):
            if millerRabin(s) == False:
                break
            if i == 20-1:
                isPrime = True  # number is very likely to be a prime
    return s

# implementation of the miller rabin algorithm to try to determine if an integer n is prime
# (note that the miller rabin algorithm is yes biased)
# n:      int to check for primality
# return: boolean
def millerRabin(n):
    # write (n-1) = m*2^k (where m is odd)
    k = 0       # number of times we were able to divide by 2
    total = (n-1)
    while total % 2 == 0:
        # must do integer division (not float divison to avoid errors with large numbers)
        total = total // 2
        k += 1
    m = int(total)

    # choose random integer a: 1 <= a <= n-1
    a = random.randint(1, n-1)
    b = algo5_5(a, m, n)        # b = a**m % n

    # check
    if b % n == 1:
        return True

    # keep checking
    for i in range(k):
        if b % n == -1 % n:
            return True
        else:
            b = b**2 % n
    return False

# Algorithm 5.3 (Multiplicative inverse)
# see p. 168 in "Crypotgraphy Theory and Practice" ed3
# a:      modulo base (int)
# b:      number to find the inverse of
# return: int a^-1 mod b
def algo5_3(a, b):
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
# see p. 177 in "Crypotgraphy Theory and Practice" ed3
# x:      int
# c:      int
# n:      int
# return: int z = x^c mod n
def algo5_5(x, c, n):
    c = bin(c)[2::]       # convert c from a integer to binary (str of 1s and 0s)
    z = 1
    l = len(c)
    for i in range(0, l):
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
