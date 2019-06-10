#Matrix / Sequence / Vectors
prob1 = function()
{
	cat('Some R Practice')
	myMatrix = matrix(seq(1, 16), nrow=4, ncol=4)
	print(myMatrix)
	print(myMatrix[c(2,4), c(2,4)])
}

# Some Mathy Stuff, 3D PNG Image
prob2 = function() {
    cat("\nExercise 2:\n")
    x = seq(-5, 5, length.out= 100)
    y = x
    z = outer(x,y, function(x,y)cos(x^2 + y^2)/(x^2 + y^2 + 1))
    png("perspective_plot.png")
    persp(x, y, z)
    # Need to do this to close the png
    dev.off()
}

# Read CSV File, add row names if they are missing
# Can Use fix(College) to view the data easily
prob3 = function(){
	College=read.csv('College.csv')
	# Name rows by the first column
	rownames(College) = College[,1]
	# This will result in duplicate columns however, so need to delete
	College = College[, -1]
}

# Basic analytics, get the mean of a field
prob4 = function()
{
	m = mean(College[["Books"]])
	cat(sprintf("mean of books = %f\n", m))
}

# Linear Regression On Single Value
prob5 = function() {
    set.seed(1)                                 # makes random the same each time
    x1 = runif(100)
    x2 = 0.5*x1 + rnorm(100) / 10

    # creating a linear model in which y is a function of x1 and x2.
    # this follows the standard form y = B_0 + B_1 * x_1 + B_2 * x_2 + E
    y = 2 + 2*x1 + 0.3*x2 + rnorm(100)

    # Regression Line
    fit = lm(y ~ x1 + x2)
    summary(fit)

    # Get correlation
    print(cor(x1, x2))
}

# Linear Regression 
prob6 = function() {	
	attach(Boston)
	someFit = lm(rm ~ age, data=Boston)
	detach(Boston)
}












