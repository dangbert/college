# Daniel Engbert
# proj2
# STAT 355 Spring 2017


# parameters:
# sample size, mean, standard deviation
experiment1 = function(n, m, s) {
    cat("---------- Experiment1 ----------\n")
    means <- array()
    sdevs <- array()

    for (i in 1:1000) {                             # r starts indices at 1 (not 0)
        res = rnorm(n, m, s)                        # n, mean, sd
        means[i] <- mean(res)                       # store vector in list
        sdevs[i] <- sd(res)                         # store vector in list
        if (i == 1) {
            cat("first sample:\n")
            cat(sprintf("\tmean = %f\n", mean(res)))
            cat(sprintf("\tstandard deviation = %f\n\n", sd(res)))
        }
    }

    png("histogram1.png")
    hist(means, main="Histogram of Means (Part 1)")
    cat("Experiment Distribution Results:\n")
    cat(sprintf("\tmean = %f\n", mean(means)))
    cat(sprintf("\tstandard deviation = %f\n\n\n\n", sd(sdevs)))
}


# parameters:
# number of observations, size (number of trials), prob of success on each trial, experiment number
experiment2 = function(n, s, p, ex) {
    cat(sprintf("---------- Experiment%d ----------\n",ex))
    means <- array()
    sdevs <- array()

    for (i in 1:1000) {
        res = rbinom(n, s, p)                       # num observations, size, prob of success
        means[i] <- mean(res)
        sdevs[i] <- sd(res)
        if (i == 1) {
            cat("first sample:\n")
            print(res)
            cat(sprintf("\tmean = %f\n", mean(res)))
            cat(sprintf("\tstandard deviation = %f\n\n", sd(res)))
        }
    }

    png(sprintf("histogram%d.png", ex))
    hist(means, main=sprintf("Histogram of Means (Part %d)", ex))
    cat("Experiment Distribution Results:\n")
    cat(sprintf("\tmean = %f\n", mean(means)))
    cat(sprintf("\tstandard deviation = %f\n\n\n\n", sd(sdevs)))
}


experiment1(40, 3, 2)
experiment2(10, 15, 0.15, ex=2)
experiment2(10, 120, 0.15, ex=3)
