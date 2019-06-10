# Lab 6: Subset Selection
# https://www.csee.umbc.edu/~cmarron/cmsc478/labs/lab6/lab06.shtml
# CMSC 478 Fall 2017

options(warn=1)
library(ISLR)
library(leaps)
library(gam)       # install.packages("gam")
library(splines)

# output the results of problem 1
prob1 = function() {
    cat("---------------Exercise 1:---------------\n")

    data = College                              # ISLR college dataset
    data = data[sample(nrow(data)),]            # shuffle rows in data frame
    data["Private"]  = ifelse(data$Private  == "Yes", 1, 0)

    train = data[(1:500),]                      # training data
    test = data[(501:nrow(data)),]                 # test data
    #fix(data)

    # forward selction
    # nvmax is ncol(train) - 1 (not using Outstate)
    model = regsubsets(Outstate~., data=train, method="forward", nvmax=17)
    summary(model)

    val_errors = rep(NA, 17)
    x_test = model.matrix(Outstate~., data=test)
    for (i in 1:17) {
        co = coef(model, id=i)
        pred = x_test[,names(co)]%*%co
        #print(pred)
        #print(test)
        val_errors[i] = mean((test$Outstate - pred)^2)
    }
    print(val_errors)


    # do a plot (doesn't work rn)
    #plot(sqrt(val_errors), ylab="Root MSE", ylim=c(300,400), pch=19, type="b")
    #legend("topright", lengend=c("Training", "Validation"), col=c("blue", "black"), pch=19)

    return(model)

    cat("\n---------------Exercise 2:---------------\n")
    x = 4
    # don't need to smooth Private
    # TODO: use the variables from the model in prob3 instead
    gam1 = gam(Outstate~ Private + s(Apps, df=x) + s(Accept, df=x) + s(Top10perc, df=x) + s(F.Undergrad, df=x) + s(Room.Board, df=x) + s(Personal, df=x) + s(Terminal, df=x) + s(perc.alumni, df=x) + s(Expend, df=x) + s(Grad.Rate, df=x), data=train)

}

prob2 = function(model) {

}

prob3 = function() {
    cat("\n---------------Exercise 3:---------------\n")
    data = College                              # ISLR college dataset
    data = data[sample(nrow(data)),]            # shuffle rows in data frame
    data["Private"]  = ifelse(data$Private  == "Yes", 1, 0)

    train = data[(1:500),]                      # training data
    test = data[(501:nrow(data)),]              # test data
    #fix(data)
    #print(names(train))

    # these variables were determined by James when he used forward selection (what I tried to do in prob1)
    # for some reason this isn't straight when plotted
    model = lm(Outstate ~ Accept, data=train)
    print(model)
    plot(model)

    # do MSE with test data
    result = predict(model, test, type='response')
    result = as.vector(result)                  # predicted values of r
    mse = mean((result - test$Outstate)^2)             # mean squared error

    cat(sprintf("mse = %f\n", mse))
}

model = prob1()
#prob2(model)
prob3()
