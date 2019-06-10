# Daniel Engbert
# STAT 355 Spring 2017
# problem 3.58

# X~Bin(10, p) where p = proportion of defectives

# returns the probability that the sample will be accepted when the actual
# proportion of defectives is p and the sample size is n
# (a sample is accepted if the number of defective components is <= allowed)
getProb = function(p, allowed, n) {
    dist = pbinom(seq(0, n), n, p)
    return(dist[1 + allowed])                    # return the probability as a double
}

# do parts a and b of the problem
# using the specified parameters
a_b = function(allowed, n) {
    cat("\nwhen max allowed defectives = ", allowed, " and sample size = ", n, "\n")
    # part (a)
    p = 0.01
    prob = getProb(p, allowed, n)
    cat("p = ", p, "\tP(batch accepted) = ", prob, "\n")

    p = 0.05
    prob = getProb(p, allowed, n)
    cat("p = ", p, "\tP(batch accepted) = ", prob, "\n")

    p = 0.10
    prob = getProb(p, allowed, n)
    cat("p = ", p, "\tP(batch accepted) = ", prob, "\n")

    p = 0.20
    prob = getProb(p, allowed, n)
    cat("p = ", p, "\tP(batch accepted) = ", prob, "\n")

    p = 0.25
    prob = getProb(p, allowed, n)
    cat("p = ", p, "\tP(batch accepted) = ", prob, "\n")


    # part (b)
    f = Vectorize(getProb)
    fname = paste("graph", allowed, "_", n, ".png")
    png(fname)

    curve(f(x, allowed, n), from=0, to=1, xlab="p", ylab="P(batch is accepted)")
}

# do parts (a) and (b)
a_b(2, 10)

# do part (c)
a_b(1, 10)

# do part (d)
a_b(2, 15)

# part (e)
# the sampling plan from part (d) appears to be the most satisfactory
# because based on the graphs it does the best job rejecting batches
# with a proportion of more than 0.1 defective components, while also
# doing a good job of accepting batches with a proportion of at most
# 0.1 defective components.
