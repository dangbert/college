* Simple example for testing/understanding:

````python
# modify script to use this main:
def main():
    # Elliptic curve parameters for Secp256k1 (y^2 = x^3 + ax + b)
    p = 127
    a, b = 1, 26
    Gx = 2
    Gy = 6
    E = (p, a, b, (Gx, Gy))  # P = (Gx, Gy)
    #tests(E) # perform some tests that everything is working
    #print(doubleAndAdd((2,6), 22, E))
    print(addPoints((91, 18), (18,62), E))
    return
````
