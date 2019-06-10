# Dan Engbert
# CMSC 478 - Fall 17

# HOW TO RUN:
#     place the 7 data files (e.g. path1_data.csv) in a folder called "data"
#     (so that the data folder and this file are in the same directory)
#     then run the code with the shell command:  Rscript code.R

# NOTES:
# I choose to use the glm function for my model because intuitively I figured there would be a
# linear relationship between the distance and the sensor values; and my results were promising. 
# First I tried using two models (one for the x location and one for the y), and then using those
# estimates to calculate the distance, but the average MSE was very high.
#
# So next I instead made one model to predict the distance (r) directly.
# I used only the variables S1 ... S13 at first
# and I briefly squared all the variables in the model to see what would happen
# it was really bad so I made all the variables linear instead and saw much better results:
# the average MSE over 1000 trials was ~4.92
#
# At this point I was just setting "NA" values to 0 in the data (which makes it seem like the
# object is on top of the sensor).
#
# To improve this I added dummy variables to the model (T1 ... T13) which were set to 1 if the
# sensor could see the object (i.e. wasn't "NA") and 0 otherwise.
#
# I then ran my k-fold function 1000 time (shuffling the data each time) and the average MSE was
# 2.345 which corresponds to the prediction tending to be off by 1.5 units
#
# next I tried to see if any of the variables weren't important: to the model
# I removed S1 and T1 and did 75 10-fold tests
# then I put S1 and T1 back and removed S2, T2 and redid the tests
# I repeated this for all the sets of variables (1 to 13)
# the average MSE was significantly worse everytime
# except for after removing S12 and T12 (average MSE was 2.34 which was the same as with all
# variables present). This wasn't any better so I decided to leave all the variables in the model.



# reads the data from the csv file
# runs a k-fold cross validation test
# then creates a model using all the data
main = function() {
    allData = readData()

    # 10-fold testing
    cat("### Testing: ###\n")
    allData = allData[sample(nrow(allData)),]   # shuffle rows in data frame
    kfold(allData, 10)

    # now create a model using all the available data:
    cat("\n\n### Final Model: ###\n")
    model = makeModel(allData)
    print(model)
}

# read in a test data file and output the predictions to a csv
evalTestData = function(filename) {
    # create a model using all the available data:
    allData = readData()
    allData = allData[sample(nrow(allData)),]   # shuffle rows in data frame
    model = makeModel(allData)
    test = read.csv(filename)                   # read in test data

    # add dummy variables to test data and replace "NA" with 0
    test = prepareData(test)

    # predict values of r
    result = as.vector(predict(model, test, type='response'))
    test["r"] = result
    test["close"]  = ifelse(test$r  <= 3, "TRUE", "FALSE")
    #fix(test)
    write.csv(test, file="output.csv")
    cat("Results outputed to output.csv\n")
}

# create a model using the provided (training) data frame
makeModel = function(data) {
    model = glm(r ~ S1 + S2 + S3 + S4 + S5 + S6 + S7 + S8 + S9 + S10 + S11 + S12 + S13 + T1 + T2 + T3 + T4 + T5 + T6 + T7 + T8 + T9 + T10 + T11 + T12 + T13, data=data)
    return(model)
}


# read the 7 csv files and return the combined data frame
# create dummy variables as well
readData = function() {
    # combine data in all the files
    # note that all the csv files have 50 data points except for #6 which has 30 (330 rows total)
    allData = read.csv("data/path1_data.csv")
    for (i in 2:7) {
        # data frame for this csv file
        tmp = read.csv(sprintf("data/path%d_data.csv", i))
        allData <- rbind(allData, tmp)          # append this data to the allData data frame
    }
    # allData now contains the rows from all the csv files combined
    allData = prepareData(allData)
    #fix(allData)

    return(allData)
}

# add dummy variables to data frame and replace NAs with 0
prepareData = function(data) {
    # create dummy variables to indicate whether each sensor could see the object
    data["T1"]  = ifelse(data$S1  == "NA", 0, 1)
    data["T2"]  = ifelse(data$S2  == "NA", 0, 1)
    data["T3"]  = ifelse(data$S3  == "NA", 0, 1)
    data["T4"]  = ifelse(data$S4  == "NA", 0, 1)
    data["T5"]  = ifelse(data$S5  == "NA", 0, 1)
    data["T6"]  = ifelse(data$S6  == "NA", 0, 1)
    data["T7"]  = ifelse(data$S7  == "NA", 0, 1)
    data["T8"]  = ifelse(data$S8  == "NA", 0, 1)
    data["T9"]  = ifelse(data$S9  == "NA", 0, 1)
    data["T10"] = ifelse(data$S10 == "NA", 0, 1)
    data["T11"] = ifelse(data$S11 == "NA", 0, 1)
    data["T12"] = ifelse(data$S12 == "NA", 0, 1)
    data["T13"] = ifelse(data$S13 == "NA", 0, 1)

    data[is.na(data)] <- 0                      # now replace all "NA" values with 0
    return(data)
}


# apply the model to the test data and return the MSE
testModel = function(model, test) {
    # apply the model to the test data
    result = predict(model, test, type='response')
    result = as.vector(result)                  # predicted values of r

    #cat("prediction, actual\n")
    #for (i in 1:length(result)) {
    #    cat(sprintf("%f, %f\n", result[i], test$r[i]))
    #}

    mse = mean((result - test$r)^2)             # mean squared error
    #cat(sprintf("mse = %f\n", mse))
    return(mse)
}


# do k-fold cross-validation
# (it is assumed that the data frame has already been shuffled)
# last group will be the same size as the others (so some elements might not be used)
kfold = function(allData, k) {
    cat(sprintf("\ndoing %d-fold cross-validation test:\n", k))
    total = nrow(allData)                       # total number of data points
    size = floor(total / k)                     # size of each group

    avg = 0
    end = 0
    for (i in 0:(k-1)) {
        # use group i for testing
        start = end + 1                         # start index of test group
        end = start + size -1                   # end index of the test group

        test = allData[(start:end),]            # test data
        train = allData[-(start:end),]          # training data

        # train and test the model
        model <- makeModel(train)
        mse = testModel(model, test)
        avg = avg + mse
        cat(sprintf("\tgroup %d test MSE: %f\n", i, mse))
    }
    avg = avg / k
    cat(sprintf("\taverage MSE: %f\n", avg))
    return(avg)
}

main()
#evalTestData("data/test_path_data.csv")
