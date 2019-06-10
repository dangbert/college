data.auto = read.csv("Auto.csv", header=T, na.strings ="?")
data.auto = na.omit(data.auto)
attach(data.auto)

data.auto.median = median(mpg)
data.auto["mpg01"] = ifelse(mpg > data.auto.median, 1, 0)
attach(data.auto)

pairs(data.auto)

boxplot(displacement~mpg01,data=data.auto, xlab="mpg01", ylab="displacement")
boxplot(horsepower~mpg01,data=data.auto, xlab="mpg01", ylab="horsepower")
boxplot(weight~mpg01,data=data.auto, xlab="mpg01", ylab="weight")
boxplot(acceleration~mpg01,data=data.auto, xlab="mpg01", ylab="acceleration")

data.shuffled = data.auto[sample(nrow(data.auto)),]
data.train = head(data.shuffled, 300)
data.test = tail(data.shuffled, 92)

detach(data.auto)
attach(data.train)

#Logistic Regression
log.fit = glm(mpg01 ~ acceleration + weight + horsepower + displacement, data=data.train, family = binomial)
log.probs = predict(log.fit, data.test, type='response')
log.pred = ifelse(log.probs > .5, 1, 0)

print("0: mpg01 <= 1")
print("1: mpg01 > 1")
print("Test Data Matrix: Logistic Regression")
log.pred.matrix = table(log.pred, data.test$mpg01)
log.pred.matrix

print("Test Error: Logistic Regression")
mean(log.pred != data.test$mpg01)

#LDA
lda.fit = lda(mpg01 ~ acceleration + weight + horsepower + displacement, data=data.train)
lda.pred = predict(lda.fit, data.test)$class

print("0: mpg01 <= 1")
print("1: mpg01 > 1")
print("Test Data Matrix: LDA")
lda.pred.matrix = table(lda.pred, data.test$mpg01)
lda.pred.matrix

print("Test Error: LDA")
mean(lda.pred != data.test$mpg01)

#QDA
qda.fit = qda(mpg01 ~ acceleration + weight + horsepower + displacement, data=data.train)
qda.pred = predict(qda.fit, data.test)$class

print("0: mpg01 <= 1")
print("1: mpg01 > 1")
print("Test Data Matrix: QDA")
qda.pred.matrix = table(qda.pred, data.test$mpg01)
qda.pred.matrix

print("Test Error: QDA")
mean(qda.pred != data.test$mpg01)
