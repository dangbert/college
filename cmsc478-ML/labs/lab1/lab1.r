# Daniel Engbert
# Lab 1: Introduction to R
# https://www.csee.umbc.edu/~cmarron/cmsc478/labs/lab1/lab01.shtml
# CMSC 478 Fall 2017

options(warn=1)

# output the results of problem 1
prob1 = function() {
    cat("Exercise 1:\n")
    # create a matrix
    C = matrix(
        seq(1,16),
        nrow=4,
        ncol=4
    )
    print(C)

    # get the submatrix of C with rows 2 and 4 and columns 2 and 4
    D <- C[c(2,4), c(2,4)]
    print(D)

}

# this function written by Bernadeht E
prob2 = function() {
    cat("\nExercise 2:\n")
    x = seq(-5, 5, length.out= 100)
    y = x
    z = outer(x,y, function(x,y)cos(x^2 + y^2)/(x^2 + y^2 + 1))
    png("output/2-perspective_plot.png")
    persp(x, y, z)
}

# this function written by Gabrielle B, modified by Dan
prob3 = function() {
    cat("\nExercise 3:\n")
    x = rnorm(9)                                # array of 9 random values
    A = matrix(x, nrow=3, ncol=3)
    print(A)

    eignA=eigen(A)                              # compute eigen vectors and values
    print(eignA)

    # verify that the eigen vectors and eigenvalues satisfy Ax_i = (lambda) * x_i
    cat("\neigen vector 1 check:\n")
    print(A %*% eignA$vectors[,1])
    print(eignA$values[1] * eignA$vectors[,1])

    cat("\neigen vector 2 check:\n")
    print(A %*% eignA$vectors[,2])
    print(eignA$values[2] * eignA$vectors[,2])

    cat("\neigen vector 3 check:\n")
    print(A %*% eignA$vectors[,3])
    print(eignA$values[3] * eignA$vectors[,3])
}

# do problems 4 through 7
# (not entirely complete)
prob4_5_6_7 = function() {
    # prob4
    cat("\nExercise 4:\n")
    College=read.csv("data/College.csv")
    rownames(College) = College[,1]
    #fix(College)                                # if you want to make changes to the data?

    College = College[, -1]

    m = mean(College[["Books"]])
    cat(sprintf("mean of books = %f\n", m))

    # prob5
    cat("\nExercise 5:\n")
    # so that you can access the variables of the data set directly
    # (e.g. Enroll rather than College$Enroll)
    attach(College)
    # create graphs of all the specified columns when paired
    png("output/5-pairs_graph.png")
    pairs(College[,c(5,6,13,15,17,18)])


    # prob6
    cat("\nExercise 6:\n")
    # create histogram
    # Grad is a column from College that we can access do to the "attach" above
    png("output/6-grad_rate_histogram.png")
    hist(Grad.Rate)


    # prob7
    cat("\nExercise 7:\n")
    # count the number of values in the Top10perc column that are > 50
    count = 0
    for (i in 1:777) {
        if (Top10perc[i] > 50) {
            count = count + 1
        }
    }
    cat(sprintf("count = %d\n", count))

    # smarter way to do the same thing
    # creates a vector of true false values and adds the trues
    count = sum(Top10perc > 50)
}


# run the tests
prob1()
prob2()
prob3()
prob4_5_6_7()
