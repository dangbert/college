# Daniel Engbert
# proj3
# STAT 355 Spring 2017

library(pwr)
library(ggplot2)
options(warn=1)

# make a conclusion based on the p-value and alpha level
conclude = function(pval, alpha) {
    if (pval < alpha) {
        cat(sprintf("We reject the null hypthothesis at the %.3f level.\n", alpha))
    }
    else {
        cat(sprintf("We fail to reject the null hypthothesis at the %.3f level.\n", alpha))
    }
}


# output the results of problem 1
prob1 = function() {
    cat("---------- Problem1 ----------\n")
    z <- (73.2 - 72.4) / (2.1 / sqrt(35))
    pval <- 2 * (1 - pnorm(z))

    cat(sprintf("TS = %f\n", z))
    cat(sprintf("p-value = %f\n", pval))
    conclude(pval, 0.05)
}


prob2 = function() {
    cat("\n\n---------- Problem2 ----------\n")
    x_bar <- 73.2
    s <- 7.9
    n <- 12
    u0 = 75

    TS <- (x_bar - u0) / (s / sqrt(n))
    pval <- pt(TS, n-1)                         # n-1 degrees of freedom

    cat(sprintf("TS = %f\n", TS))
    cat(sprintf("p-value = %f\n", pval))
    conclude(pval, 0.05)
}


prob3 = function() {
    cat("\n\n---------- Problem3 ----------\n")
    data <- c(66, 63, 64, 62, 65)
    x_bar = mean(data)
    s = sd(data)
    n = length(data)
    u0 = 60

    TS <- (x_bar - u0) / (s / sqrt(n))
    pval <- 2 * pt(TS, n-1, lower=FALSE)        # n-1 degrees of freedom

    cat(sprintf("TS = %f\n", TS))
    cat(sprintf("p-value = %f\n", pval))
    conclude(pval, 0.05)

    # create boxplot of the data
    png("output/3-boxplot.png")
    boxplot(data, ylab="Dog Weights (lbs)")
    title("3. Box Plot")

    # create normal probability plot
    png("output/3-normal_prob_plot")
    qqnorm(data, main="3. Normal Q-Q Plot")
    qqline(data, col="red")
}


prob4 = function() {
    cat("\n\n---------- Problem4 ----------\n")
    u0 = 5.2
    v0 = 0.8                                    # expected variance
    n = 15
    x_bar = 5.4
    s = 1.0

    # two sided t-test because population standard deviation is not known (with certainty)
    cat("H0: u != 5.2\n")
    TS <- (x_bar - u0) / (s / sqrt(n))
    pval <- 2 * pt(TS, n-1, lower=FALSE)        # n-1 degrees of freedom

    # print results
    cat(sprintf("TS = %f\n", TS))
    cat(sprintf("p-value = %f\n", pval))
    conclude(pval, 0.05)
}


prob5 = function() {
    u0 = 2.5                                    # at most 2.5 volts
    pop_sd = 0.100
    pop_u = 2.6

    #d = abs(u0 - pop_u)
    #pwr.t.test(n=NULL, d=d, sig.level=0.05, power=0.95, type="one.sample")

    png("output/5-power_curve.png")
    nvals <- c(5, 10, 15)
    deltas <- seq(0, 0.2, length.out=30)

    plot(deltas, seq(0, 1, length.out=length(deltas)), xlab="Difference", ylab="Power",
    main="Power Curve for 1-Sample t Test\nwith Varying Sample Sizes", type="n")


    for (i in 1:3) {
        powvals <- sapply(deltas, function (x) power.t.test(n=nvals[i], delta=x, power=NULL, sd=pop_sd, sig.level=0.05)$power)
        lines(deltas, powvals, lwd=2, col=i)
    }
    legend("topleft", lwd=2, col=1:3, legend=nvals)

    # plot the power curve for one sample t-test as figure 8.9

}


prob6 = function(u) {
    n = 4
    samples = 10000
    u0 = 20

    #data <- matrix( rnorm(n=n, mean=20,sd=2), samples, n)
    #prmatrix(data)

    pvals <- array()
    for (i in 1:samples) {
        data <- rnorm(n=n, mean=20,sd=2)        # 4 random data points from this normal distribution
        #prmatrix(data)

        x_bar <- mean(data)
        s <- sd(data)

        TS <- (x_bar - u) / (s / sqrt(n))
        # calculate the pvalue of this set of data
        pval <- pt(TS, n-1)                     # n-1 degrees of freedom
        pvals[i] <- pval                        # store pvalue in array
    }

    png(sprintf("output/6-histogram_%d.png", u))
    title = sprintf("6. Histogram (u = %d)", u)
    hist(pvals, breaks=20, main=title)
}


# run the tests
prob1()
prob2()
prob3()
prob4()
prob5()

prob6(20)
prob6(21)
prob6(22)
