# Daniel, Ryan, James, Michael
# https://www.csee.umbc.edu/~cmarron/cmsc478/labs/lab5/lab05.shtml
# CMSC 478 Fall 2017

options(warn=1)

# output the results of problem 1
# all the other problems are just modified versions of this problem
prob1_2 = function() {
    library(ISLR)
    set.seed(1)
    cat("---------------Exercise 1:---------------\n")

    def=read.csv("data/Default.csv")
    attach(def)
    def["res"] = ifelse(default == "Yes", 1, 0)
    attach(def)
    train=sample(10000,5000)

    # glm without library=binomial is the same as lm
    # instead we are doing logistical regression
    fit = glm(res ~ balance + income, family=binomial, data=def, subset=train)
    fit_probs = predict(fit, def[-train], type='response')
    fit_pred = ifelse(fit_probs > 0.5, 1, 0)    # set threshold
    print(fit)
    cat("###table:\n")
    print("num rows = " + nrow(def[-train]))
    print(table(fit_pred, def[-train]$res))

    # get the MSE
   #print(mean((res-predict(fit, def))[-train]^2))
    print(mean(fit_pred[-train]^2))
    print(summary(fit))



}

prob3 = function() {

}


prob4 = function() {

}

# do the problems
prob1_2()
#prob3()
#prob4()
