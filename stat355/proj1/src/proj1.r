# Daniel Engbert
# STAT 355 Spring 2017


# rbinom(n, size, prob) generates random deviates
# n    = number of experiments
# size = number of trials (per experiment)
# prob = probability of success on each trial
data = rbinom(1000, 20, 0.4)
#print(data)                  # print raw data

# create histogram (pdf file) of the generated data
png("histogram.png")
hist(data)

# compute the mean, variance, and standard deviation
mean(data)
var(data)
sd(data)

# compute the 5 number summary
cat("\n")
summary(data)
cat("\n")

# count the frequencies and calculate the relative frequencies of the data:
freq = table(factor(data, levels=0:20)) # frequencies of the data (including values of 0)
rfreq = round(freq/1000, digits=4)      # relative frequencies
tab = rbind(freq, rfreq)                # combine into 1 table

# calculate P(X = x) and compare with relative frequencies of generated data
PDF = dbinom(seq(0, 20), 20, 0.4)       # PDF of each point
PDF = setNames(PDF, 0:20)               # add labels to each column
PDF = round(PDF, digits=4)

tab = rbind(tab, PDF)                   # add PDF row to table
print(tab)
