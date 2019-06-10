# Daniel Engbert
# Lab 2: Linear Regression
# https://www.csee.umbc.edu/~cmarron/cmsc478/labs/lab2/lab02.shtml
# CMSC 478 Fall 2017

options(warn=1)

# output the results of problems 1-4
prob1_4 = function() {
    cat("Exercise 1:\n")
    set.seed(1)                                 # makes random the same each time
    x1 = runif(100)
    x2 = 0.5*x1 + rnorm(100) / 10
    # creating a linear model in which y is a function of x1 and x2.
    # this follows the standard form y = B_0 + B_1 * x_1 + B_2 * x_2 + E
    y = 2 + 2*x1 + 0.3*x2 + rnorm(100)

    cat("Regression coefficients:\n")
    fit = lm(y ~ x1 + x2)
    print(fit)


    cat("\nExercise 2:\n")
    cat("Correlation between x1 and x2:\n")
    # print the R value:
    print(cor(x1, x2))
    # scatter plot x1 vs x2
    png("output/2-plot_x1_x2.png")
    plot(x1, x2)


    cat("\nExercise 3:\n")
    # see the fit printed in exercise 1
    cat("3a) B_0=2.13\t B_1=1.44\t B_2=1.01\n")
    # the true values of B_0 - B_2 are given in the definition of y (from exercise 1)
    cat("B_0 was really close to the true value (2)\n")
    cat("B_1 was relatively close to the true value (2)\n")
    cat("B_2 was quite off from the true value (0.3)\n")
    print(summary(fit))
    cat("\n3c) Since the p-value for the B_1 (the coefficient for x_1) is 0.0487 < 0.05 we reject the null hypothesis H_0: B_1 = 0\n")
    cat("3c) Since the p-value for the B_2 (the coefficient for x_2) is 0.3754 > 0.05 we fail to reject the null hypothesis H_0: B_2 = 0\n")


    cat("\nExercise 4:\n")
    fit_x1 = lm(y ~ x1)
    cat("fit y ~ x_1:\n")
    print(summary(fit_x1))
    fit_x2 = lm(y ~ x2)
    cat("\nfit y ~ x_2:\n")
    print(summary(fit_x2))
    cat("4a, 4b) you can reject the null hypothesis in both cases because the p-values are < 0.05\n")
    cat("4c) These results don't contradict the results from exercise 3 because x_1 and x_2 are correlated.\n")
}

# contributed by Gabrielle and Bernadeht:
prob5_7 = function() {
    cat("\nExercise 5:\n")
    library(MASS)
    ##5
    #~. means use all the other variables in the Boston
    # data frame except crim
    fit1=lm(crim~.,Boston)
    print(summary(fit1))


    ##6
    cat("\nExercise 6:\n")
    fit2=lm(crim~zn,data=Boston)
    # Brief summary
    print(summary(fit2))

    fit3=lm(crim~indus,data=Boston)
    # Brief summary
    print(summary(fit3))

    fit4=lm(crim~nox,data=Boston)
    # Brief summary
    print(summary(fit4))

    fit5=lm(crim~medv,data=Boston)
    # Brief summary
    print(summary(fit5))

    names(Boston)
    # Gives the correlation between the predictors
    cor(Boston[,c(2,3,5,14)])
    png("output/6-plot_boston.png")
    plot(Boston$zn,Boston$crim)


    ##7
    cat("\nExercise 7:\n")
    fit6=lm(crim~zn+I(zn^2),Boston)
    print(summary(fit6))

    fit7=lm(crim~indus+I(indus^2),Boston)
    print(summary(fit7))

    fit8=lm(crim~nox+I(nox^2),Boston)
    print(summary(fit8))

    fit9=lm(crim~medv+I(medv^2),Boston)
    print(summary(fit9))
}


# run the tests
prob1_4()
cat("\n-----------------------------------------------\n")
prob5_7()
