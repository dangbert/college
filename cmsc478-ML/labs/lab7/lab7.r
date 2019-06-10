# Lab 7: Regularization and Dimension Reduction

# https://www.csee.umbc.edu/~cmarron/cmsc478/labs/lab6/lab06.shtml
# CMSC 478 Fall 2017

options(warn=1)
library(ISLR)
library(glmnet)                                 # install with: install.packages("leaps")

# output the results of problem 1
prob1 = function() {
    cat("---------------Exercise 1:---------------\n")
    college=read.csv("data/College.csv")
    attach(college)
    college["PAccept"] = (Accept / Apps)        # add a "PAccept" column to the dataset
    #fix(college)                               # view the data

    reg_full = regsubsets(PAccept ~.-Apps-Accept, data=college, nvmax=16)
    reg_summary = summary(reg_full)
    print(reg_summary)

    detach(college)
}

prob1()
