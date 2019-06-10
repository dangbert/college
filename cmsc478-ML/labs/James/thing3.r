#Data=read.csv('weekly_stock_data.csv')
#attach(Data)
#summary(Data)
#cor(Data[, -10])
#pairs(Data)

#All variables
#log.fit = glm(Direction ~ Lag1 + Lag2 + Lag3 + Lag4 + Lag5 + Volume, data=Data, family = binomial)

#log.fit = glm(Direction ~ Lag1 + Lag2 + Lag3 + Lag4 + Volume, data=Data, family = binomial)

#log.fit = glm(Direction ~ Lag1 + Lag2 + Lag3 + Lag4, data=Data, family = binomial)

#log.fit = glm(Direction ~ Lag1 + Lag2 + Lag4, data=Data, family = binomial)

#log.fit = glm(Direction ~ Lag1 + Lag2, data=Data, family = binomial)

train = (Year<2009)
test = Data[!train,]

log.fit = glm(Direction ~ Lag2, data=Data, family = binomial, subset=train)

log.pred = predict(log.fit, type='response')
log.probs = ifelse(log.pred >.5, "Up", "Down")
#pthresh = log.pred > .5
table(log.probs, Direction[!train])
mean(log.probs == Direction[!train])

model = lda(Direction ~ Lag2 + Lag2:Lag1, data=Data, subset=train)
prediction = predict(model, test)$class
table(prediction, Direction[!train])
mean(prediction == Direction[!train])

model2 = qda(Direction ~ Lag2  + Lag2:Lag1, data=Data, subset=train)
prediction2 = predict(model2, test)$class
table(prediction2, Direction[!train])
mean(prediction2 == Direction[!train])

#detach(Data)