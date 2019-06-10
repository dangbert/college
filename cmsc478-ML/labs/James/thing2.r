attach(Smarket)

train = (Year<2005)
test = Smarket[!train,]

model = lda(Direction ~ Lag1 + Lag2, data=Smarket, subset=train)
prediction = predict(model, test)$class
table(prediction, Direction[!train])

detach(Smarket)
