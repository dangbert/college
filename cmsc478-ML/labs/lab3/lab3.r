# Daniel, Bernadeht, Gabrielle, Ryan, James
# Lab 3: Linear Regression
# https://www.csee.umbc.edu/~cmarron/cmsc478/labs/lab3/lab03.shtml
# CMSC 478 Fall 2017

options(warn=1)

# output the results of problem 1
# all the other problems are just modified versions of this problem
prob1 = function() {
    cat("Exercise 1:\n")
    # training data
    threat_data=read.csv("data/threat_data.csv")
    attach(threat_data)
    glm.fit = glm(threat ~ time + s1 + s2 + s3, family=binomial)
    print(glm.fit)

    # gives probability of each point not being a threat (1000 points)
    p = predict(glm.fit, type="response")
    #print(p)
    # use 0.95 as the threshold
    pthresh = p > 0.95
    #print(pthresh)

    # test data
    threat_test=read.csv("data/threat_data_test.csv")

    # confusion matrix of training data (FALSE means it's a threat)
    # first column is all the actual threats
    # first row is all the predicted threats
    print(table(pthresh, threat))
    # using the model from the training data on the test data this time:
    p2 = predict(glm.fit, newdata=threat_test, type="response")

    # test data
    pthresh2 = p2 > 0.95
    print(table(pthresh2, threat_test$threat))  # use the threat column in the test data
    detach(threat_data)
}

prob2 = function() {
    # same as exercise 1 except don't use time in the model
    cat("\nExercise 2:\n")
    # training data
    threat_data=read.csv("data/threat_data.csv")
    attach(threat_data)
    glm.fit = glm(threat ~ s1 + s2 + s3, family=binomial)
    print(glm.fit)

    # gives probability of each point not being a threat (1000 points)
    p = predict(glm.fit, type="response")
    #print(p)
    # use 0.95 as the threshold
    pthresh = p > 0.95
    #print(pthresh)

    # test data
    threat_test=read.csv("data/threat_data_test.csv")

    # confusion matrix of training data
    print(table(pthresh, threat))
    # using the model from the training data on the test data this time:
    p2 = predict(glm.fit, newdata=threat_test, type="response")

    # test data
    pthresh2 = p2 > 0.95
    print(table(pthresh2, threat_test$threat))  # use the threat column in the test data
    detach(threat_data)
}

prob3 = function() {
    # try training with smaller chunks of the training data (250 points)
    cat("\nExercise 3:\n")

    cat("\nChunk1:\n")
    # training data
    threat_data=read.csv("data/threat_data.csv")
    # only include first 250 data points
    threat_data250 = threat_data[1:250,]
    attach(threat_data250)

    glm.fit = glm(threat ~ s1 + s2 + s3, family=binomial)
    print(glm.fit)

    # gives probability of each point not being a threat (1000 points)
    p = predict(glm.fit, type="response")
    #print(p)
    # use 0.95 as the threshold
    pthresh = p > 0.975
    #print(pthresh)

    # test data
    threat_test=read.csv("data/threat_data_test.csv")

    # confusion matrix of training data
    print(table(pthresh, threat))
    # using the model from the training data on the test data this time:
    p2 = predict(glm.fit, newdata=threat_test, type="response")

    # test data
    pthresh2 = p2 > 0.975
    print(table(pthresh2, threat_test$threat))  # use the threat column in the test data
    detach(threat_data250)


    cat("\nChunk2:\n")
    # training data
    threat_data=read.csv("data/threat_data.csv")
    # only include first 250 data points
    threat_data250 = threat_data[251:500,]
    attach(threat_data250)

    glm.fit = glm(threat ~ s1 + s2 + s3, family=binomial)
    print(glm.fit)

    # gives probability of each point not being a threat (1000 points)
    p = predict(glm.fit, type="response")
    #print(p)
    # use 0.95 as the threshold
    pthresh = p > 0.9
    #print(pthresh)

    # test data
    threat_test=read.csv("data/threat_data_test.csv")

    # confusion matrix of training data
    print(table(pthresh, threat))
    # using the model from the training data on the test data this time:
    p2 = predict(glm.fit, newdata=threat_test, type="response")

    # test data
    pthresh2 = p2 > 0.9
    print(table(pthresh2, threat_test$threat))  # use the threat column in the test data
    detach(threat_data250)


    cat("\nChunk3:\n")
    # training data
    threat_data=read.csv("data/threat_data.csv")
    # only include first 250 data points
    threat_data250 = threat_data[501:750,]
    attach(threat_data250)

    glm.fit = glm(threat ~ s1 + s2 + s3, family=binomial)
    print(glm.fit)

    # gives probability of each point not being a threat (1000 points)
    p = predict(glm.fit, type="response")
    #print(p)
    # use 0.95 as the threshold
    pthresh = p > 0.95
    #print(pthresh)

    # test data
    threat_test=read.csv("data/threat_data_test.csv")

    # confusion matrix of training data
    print(table(pthresh, threat))
    # using the model from the training data on the test data this time:
    p2 = predict(glm.fit, newdata=threat_test, type="response")

    # test data
    pthresh2 = p2 > 0.95
    print(table(pthresh2, threat_test$threat))  # use the threat column in the test data
    detach(threat_data250)


    cat("\nChunk4:\n")
    # training data
    threat_data=read.csv("data/threat_data.csv")
    # only include first 250 data points
    threat_data250 = threat_data[751:1000,]
    attach(threat_data250)

    glm.fit = glm(threat ~ s1 + s2 + s3, family=binomial)
    print(glm.fit)

    # gives probability of each point not being a threat (1000 points)
    p = predict(glm.fit, type="response")
    #print(p)
    # use 0.95 as the threshold
    pthresh = p > 0.95
    #print(pthresh)

    # test data
    threat_test=read.csv("data/threat_data_test.csv")

    # confusion matrix of training data
    print(table(pthresh, threat))
    # using the model from the training data on the test data this time:
    p2 = predict(glm.fit, newdata=threat_test, type="response")

    # test data
    pthresh2 = p2 > 0.95
    print(table(pthresh2, threat_test$threat))  # use the threat column in the test data
    detach(threat_data250)
}


prob4 = function() {
    cat("\nExercise 4:\n")
    # looking at the coefficients from each chunked model,
    # the coefficients don't appear to be correlated between the models
    # so there is lots of noise in the training data
}

# do the problems
prob1()
prob2()
prob3()
prob4()
