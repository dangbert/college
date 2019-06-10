# Dan Engbert
# CMSC 478 - Fall 17

# HOW TO RUN:
#   place the training data in "./data/train.csv"
#   Rscript code.r

# NOTES:
# I tried creating the randomForest with the following parameters:
# m = 5,   500 trees -> OOB: 18.21%
# m = 10,  500 trees -> OOB: 14.4%
# m = 20,  500 trees -> OOB: 13.3%
# m = 40,  500 trees -> OOB: 13.38%
# m = 45,  500 trees -> OOB: 13.33%
# m = 50,  500 trees -> OOB: 13.29%, 13.39% (2nd test)
# m = 50, 1000 trees -> OOB: 13.3%
# m = 55,  500 trees -> OOB: 13.41%
# m = 75,  500 trees -> OOB: 13.52%
# m = 100, 500 trees -> OOB: 13.8%  (bagging)
# m = 100, 100 trees -> OOB: 14.1%  (bagging)
#
# I choose to use the parameters m=20 with 500 trees because that seemed to perform the best over
# all of my tests (above).
# values of m: 20 <= m <= 55 with 500 trees appeared to all perform about the same
# Increasing the number of trees beyond 500 did not seem to have a big impact.
#
# The estimated test error rate is 13.4%
# because the OOB estimate tended to be slighly less than this (over several tests).


library(randomForest)
# reads the data from the csv file
# then creates a model using all the data
main = function() {
    allData = readData("data/train.csv")        # has 100 predictors
    model = makeModel(allData)  # create a randomForest model
    print(model)                # outputs the model OOB (out-of-bag-error)
    #importance(model)
    return(model)
}

evalTestData = function(model, filename) {
    test = readData(filename)
    result = as.vector(predict(model, test, type='response'))
    test["class"] = result

    write.csv(test, file="output.csv")
    cat("Results outputed to output.csv\n")
}

# create a model using the provided (training) data frame
makeModel = function(data) {
    # bagging is a special case of random forest where m=p
    # (m is predictor subset size, p is the number of predictors)
    # randomForest is typically when m <= sqrt(p)

    #m = floor(sqrt(ncol(data) - 1))
    m = 20
    data$class = as.factor(data$class)          # treat the class numbers as labels
    model = randomForest(class ~., data=data, mtry=m, ntree=500)
    return(model)
}


# read the training data and return the data frame
readData = function(filename) {
    allData = read.csv(filename)
    return(allData)
}


model = main()
evalTestData(model, "data/test_without_labels.csv")
