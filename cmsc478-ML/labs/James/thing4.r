#Logistic Regression

attach(Default)
#Default["res"] = ifelse(default == "Yes", 1, 0)
attach(Default)

library(ISLR)
library(boot)

data.shuffled = Default[sample(nrow(Default)),]
data.train = head(data.shuffled, 5000) 
data.test = tail(data.shuffled, 5000)

log.fit = glm(default ~ income + balance, data=data.train, family = binomial)
log.probs = predict(log.fit, data.test, type='response')
log.pred = ifelse(log.probs > .5, 1, 0)

print("0: mpg01 <= 1")
print("1: mpg01 > 1")
print("Test Data Matrix: Logistic Regression")
log.pred.matrix = table(log.pred, data.test$res)
log.pred.matrix

print("Test Error: Logistic Regression")
print(mean(log.pred != data.test$res))

cv.glm(data.train, log.fit, K=5)

boot.fn = function(data, index) {
    return (coef(lm(res ~ income + balance, data=data ,subset=index)))
}

boot(Default, boot.fn, 500)
summary(log.fit)



