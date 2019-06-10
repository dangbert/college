#!/usr/bin/env python3
# finds a hash of a string starting with 'engbert'
# such that the hash starts with 5 zeros

import hashlib

def main():
    # find solution:
    target = "engbert"
    res = mine(target)

    # print out solution:
    print("\nsolution: nonce = " + str(res))
    print("hash(" + target + str(res) + ") = " + str(getHash(target + str(res))))

    # save solution to file:
    f = open(target + '.txt', 'w');
    f.write(target + str(res))
    f.close()

    # verify solution as teacher will:
    fid = open('engbert.txt','r');
    hS = fid.read().encode('utf-8')
    fid.close()
    yh = hashlib.sha256(hS).hexdigest()
    if yh[0:5] == '00000':
        print('You get credit')
    else:
        print('No credit')

# returns the number that needs to be appended to a string to make it's hash start with 5 zeros
def mine(string):
    nonce = 1
    while True:
        #if nonce % 1000 == 0:
        #    print("current nonce = " + str(nonce))
        nonce += 1
        h = getHash(string + str(nonce))
        cur = (string + str(nonce)).encode('utf-8')
        h = hashlib.sha256(cur).hexdigest()
        if h[0:5] == '00000':                   # check if hash is a solution
            return nonce

# return the sha256 hash of a utf-8 string
def getHash(string):
    return hashlib.sha256(string.encode('utf-8')).hexdigest()

if __name__ == "__main__":
    main()
