#!/usr/bin/env python3
# simulates an enigma machine with 1 rotor and 1 plug in a plugboard
# known: if x = 'axe' and k = (3, 'a', 'f')  then ans = 'UOX'.

rotor = [6, 14, 7, 8, 12, 17, 9, 13, 20, 23, 16, 21, 25, 15, 24, 18, 0, 19, 5, 4, 22, 2, 10, 1, 11, 3]
def main():
    # parameters:
    s = "axed"
    key = [3, 'a', 'f']

    # convert key to all ints
    key[1] = ord(key[1]) - 97
    key[2] = ord(key[2]) - 97

    # encrypt and then decrypt s
    ans = encrypt(s, key)
    print(ans)
    ans = decrypt(ans, key)
    print(ans)

    # TODO: do this by using the fact that "weather"
    # is known to be in it
    print("\nPart1:")
    s = "PVGEQDETJWZBESHP"
    hack(s)

    print("\nPart2:")
    s = "ERXYTXLKWLBCVIFUPJRZLHNQHTCUORTXRBZHNGD"
    hack(s)

# return new letter after running through plugboard
def plug(x, key):
    if x == key[1]:
        return key[2]
    if x == key[2]:
        return key[1]
    return x

# encrypt string with key
def encrypt(s, key):
    s = s.lower()
    i = 0
    ans = ""
    for x in s:
        x = ord(x) - 97                         # convert to int
        val = (plug(x, key) + key[0] + i) % 26
        val = rotor[val]
        ans += chr(plug(val, key) + 97)
        i = i+1
    return ans.upper()

# decrypt string with key
def decrypt(s, key):
    s = s.lower()
    ans = ""
    # traverse string backwards
    for i in range(len(s)-1, -1, -1):
        val = ord(s[i]) - 97
        val = plug(val, key)

        for k in range(0, len(rotor)):
            if val == rotor[k]:
                val = k
                break

        # find param for plug that gives leads to val
        val = (val - key[0] - i) % 26
        val = plug(val, key)

        ans += chr(val + 97)
    return ans.upper()[::-1]   # reverse string

# attempt to decrypt a string
# try all possible keys, look for an output that looks english
def hack(s):
    s = s.lower()
    best = [0, [0,0,2]]
    key = [0, 0, 2]

    while key != [0, 0, 1]:
        guess = decrypt(s, key)
        m = M(guess)
        if m > best[0]:
            best[0] = m
            best[1] = list(key)
        key = advanceKey(key)
    print("best guess: key=" + str(best[1]) + ", m=" + str(best[0]))
    print(decrypt(s, best[1]))

# estimate probability of the string being english
# TODO: make sure this is implemented correctly
def M(s):
    s = s.lower()
    p = [.082,.015,.028,.043,.127,.022,.02,.061,.07,.002,.008,.04,
 .024,.067,.075,.019,.001,.06,.063,.091,.028,.01,.023,
 .001,.02,.001]
    count = [0] * 26  # number of occurences of each letter
    for c in s:
        count[ord(c) - 97] += 1
    sum = 0
    for i in range(0, 26):
        sum += p[i] * count[i]
    return sum / len(s)

# advance a key (where a key is a set of 3 integers < 26)
# TODO: this could be more efficient the last two numbers
#       represent a COMBINATION of letters not a PERMUTATION
def advanceKey(key, pos=2):
    key[pos] += 1
    if key[pos] == 26:
        key[pos] = 0 
        return advanceKey(key, pos-1)

    # last 2 values shouldn't be equal (can't plug letter into itself)
    if key[1] == key[2]:
        return advanceKey(key)
    return key

if __name__ == "__main__":
    main()
