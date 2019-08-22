 library(rpart)
 library(rpart.plot)
 library(CrossValidation) 
 install.packages("randomForest")
 library(randomForest)
 install.packages("MASS")
 library(MASS)
 install.packages("e1071")
 library(e1071)
 install.packages("nnet")
 library(nnet)
 
train <- read.csv("~/Desktop/Earnings_Numeric_Train.csv", stringsAsFactors=FALSE)
   View(train)
test <- read.csv("~/Desktop/Earnings_Test_Students _2_.csv", stringsAsFactors=FALSE)
   View(test)
earning_submission <- read.csv("~/Desktop/earning_submission.csv", stringsAsFactors=FALSE)
   View(earning_submission)   
  
boxplot(train$Earnings~ train$Major, main = "Earnings Across Different Majors", xlab = 'Major', ylab = 'Earnings', col = c("green"))
plot(train$Earnings~ train$GPA, main = "Earnings Across Different Majors", xlab = 'GPA', ylab = 'Earnings', col = c("green"))
plot(train$Earnings~ train$Number_Of_Professional_Connections, main = "Earnings and connections", xlab = 'Connections', ylab = 'Earnings', col = c("green"))
plot(train$Earnings~ train$Graduation_Year, main = "Earnings and Graduation Year", xlab = 'Graduation Year', ylab = 'Earnings', col = c("green"))
plot(train$Earnings~ train$Height, main = "Earnings and Height", xlab = 'Height', ylab = 'Earnings', col = c("green"))
plot(train$Earnings~ train$Number_Of_Credits, main = "Earnings and The Number of Credits", xlab = 'Number of Credits', ylab = 'Earnings', col = c("green"))

tree <- rpart(Earnings ~ Number_Of_Credits + Number_Of_Professional_Connections + Major, data = train, method = 'anova')
predict(tree,train)
rpart.plot(tree)
 View(tree)
prp(tree, extra = 100, box.col = "green")
CrossValidation::cross_validate(train, tree, 10, 0.8, method = "anova")





pred.tree <- predict(tree,newdata = test, type = "class")
test$Earnings <- pred.tree
earnings_submission$Earnings <- pred.tree
write.csv(earnings_submission, "earnings_submission.csv", row.names = FALSE)
