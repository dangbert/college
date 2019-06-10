# Daniel, Bernadeht, Gabrielle, Ryan, James
# Lab 4: Logistic Regression, LDA, & QDA
# https://www.csee.umbc.edu/~cmarron/cmsc478/labs/lab4/lab04.shtml
# CMSC 478 Fall 2017

options(warn=1)

# output the results of problem 1
# all the other problems are just modified versions of this problem
prob1_2 = function() {
    cat("---------------Exercise 1:---------------\n")
    stocks=read.csv("data/weekly_stock_data.csv")
    attach(stocks)
    cat("Summary\n")
    print(summary(stocks))
    cat("Correlation:\n")
    print(cor(stocks[, -10]))                   # all columns except column 10 (direction)

    png("output/1-stocks_pairs.png")
    pairs(stocks)

    # based on the correlation and the paris graph,
    # the only apparent correlations is year and volume


    cat("\n\n---------------Exercise 2:---------------\n")
    cat("***fit 1:***\n")
    gfit = glm(Direction ~ Lag1 + Lag2 + Lag3 + Lag4 + Volume, family=binomial)
    print(gfit)
    print(summary(gfit))

    cat("\n***fit 2:***\n")
    gfit = glm(Direction ~ Lag2, family=binomial)
    print(gfit)
    print(summary(gfit))

    # looking at the p-values from the summaries we can see that Lag2 is the best
    # (we tried other combinations but Lag2 was the best)
    # so we'll use just it for our predictions
    pred = predict(gfit, type='response')
    # create confusion matrix: (use 0.5 as threshold for prediction)
    # so if > 0.5 label it 'Up', otherwise predict 'Down'
    # (for now we are using the model on the training data instead of new data)
    probs = ifelse(pred > 0.5, 'Up', 'Down')
    cat("\n***confusion matrix:***\n")
    # column is guess, top row is actual direction
    print(table(probs, Direction))
    # % we get right:
    cat("\n***percent we get right:***\n")
    print(mean(probs == Direction))

    # conclusion: it does well predicting up because the data is up so much
    # the model isn't good because it's bad a predicting down
    # our data doesn't fit the logistic curve well


    detach(stocks)
}

prob3 = function() {
    cat("\n---------------Exercise 3:---------------\n")
    # lets us use LDA function
    # https://stat.ethz.ch/pipermail/r-help/2010-March/230314.html
    require("MASS")
    methods(predict)
    stocks=read.csv("data/weekly_stock_data.csv")
    attach(stocks)

    # select the testing data
    train = (Year < 2009)                       # use the years after this for testing
    test = stocks[!train,]
    # create model using training data
    model = lda(Direction ~ Lag2, data=stocks, subset=train)
    pred = predict(model, test) #$class
    print(pred)


}


prob4 = function() {
    cat("\nExercise 4:\n")
    # we tried changing the model by adding more terms
    # and doing an interaction between Lag1 and Lag2 (Lag1:Lag2)
    # and nothing made it better

}

# do the problems
prob1_2()
prob3()
prob4()
