#library(leaps)
#library(ISLR)
#attach(College)

#Exercise 1
College["PAccept"] = (Accept / Apps)
regfit.full = regsubsets(PAccept ~.-Apps-Accept, data = College, nvmax = 16)
reg.full.summary = summary(regfit.full)
names(reg.full.summary)

#Looking for MIN bic and CP
coef(regfit.full, which.min(reg.full.summary$cp))
coef(regfit.full, which.min(reg.full.summary$bic))

#Looking for MAX adjr^2 
coef(regfit.full, which.max(reg.full.summary$adjr2))

#Exercise 2a
regfit.forward = regsubsets(PAccept ~.-Apps-Accept, data = College, nvmax = 16, method = "forward")
reg.forward.summary = summary(regfit.forward)
names(reg.forward.summary)

#Looking for MIN bic and CP
coef(regfit.forward, which.min(reg.forward.summary$cp))
coef(regfit.forward, which.min(reg.forward.summary$bic))

#Looking for MAX adjr^2 
coef(regfit.forward, which.max(reg.forward.summary$adjr2))

#Exercise 2b
regfit.backward = regsubsets(PAccept ~.-Apps-Accept, data = College, nvmax = 16, method = "backward")
reg.backward.summary = summary(regfit.backward)
names(reg.backward.summary)

#Looking for MIN bic and CP
coef(regfit.backward, which.min(reg.backward.summary$cp))
coef(regfit.backward, which.min(reg.backward.summary$bic))

#Looking for MAX adjr^2 
coef(regfit.backward, which.max(reg.backward.summary$adjr2))

#Exercise 2c
plot(reg.full.summary$rsq, type = "b", col = "blue")
lines(reg.forward.summary$rsq, type = "b", col = "green")
lines(reg.backward.summary$rsq, type = "b", col = "orange")

plot(reg.full.summary$bic, type = "b", col = "blue")
lines(reg.forward.summary$bic, type = "b", col = "green")
lines(reg.backward.summary$bic, type = "b", col = "orange")

predict.regsubsets = function(object, newdata, id, ...) {
    form = as.formula(object$call[[2]])
    mat = model.matrix(form,newdata)
    coefi = coef(object, id=id)
    xvars= names(coefi)
    mat[,xvars] %*% coefi
}

#data.shuffled = College[sample(nrow(College)),]
#data.train = head(data.shuffled, 400) 
#data.test = tail(data.shuffled, 300)

#regfit.full = regsubsets(PAccept ~.-Apps-Accept, data = data.train, nvmax = 16)
#predict.regsubsets(regfit.full, data.test, 1)
set.seed(1)
k = 5
folds = sample(1:k, nrow(College), replace=TRUE)
cv.errors=matrix(NA,k,16,dimnames=list(NULL, paste(1:16)))

for(j in 1:k)
{
	best.fit = regsubsets(PAccept~. -Apps-Accept, data = College[folds !=j,], nvmax = 16)
	for(i in 1:16) 
	{
		pred=predict(best.fit ,College [folds ==j,], id=i)
		cv.errors[j,i]=mean( (College$PAccept[folds ==j]-pred)^2)
	}
}

mean.cv.errors =apply(cv.errors ,2, mean)
mean.cv.errors
par(mfrow =c(1,1))
plot(mean.cv.errors ,type='b')
