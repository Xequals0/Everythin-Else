 library(rpart)
 library(rpart.plot)
 library(CrossValidation) 

train <- read.csv("~/Desktop/Facebook_Training.csv", stringsAsFactors=FALSE)
   View(train)
test <- read.csv("~/Desktop/FBtest_students.csv", stringsAsFactors=FALSE)
   View(test)
FBsubmission <- read.csv("~/Desktop/FBsubmission.csv", stringsAsFactors=FALSE)
   View(FBsubmission)   

boxplot(train$Curse_Words_Per_Week ~ train$Party, main = "Political Party and Curse Words Per Week", xlab = 'Party', ylab = 'Curse Words Per Week', col = c("green"))
boxplot(train$Systolic_Blood_Pressure ~ train$Party, main = "Political Party and Systolic Blood Pressure", xlab = 'Party', ylab = 'Systolic Blood Pressure', col = c("green"))
boxplot(train$Diastolic_Blood_Pressure ~ train$Party, main = "Political Party and Diastolic Blood Pressure", xlab = 'Party', ylab = 'Diastolic Blood Pressure', col = c("green"))
boxplot(train$Starbucks_Visits_Per_Week ~ train$Party, main = "Political Party and Starbucks Visits Per Week", xlab = 'Party', ylab = 'Starbucks Visits Per Week', col = c("green"))
boxplot(train$Days_Since_Last_Doctor_Visit ~ train$Party, main = "Political Party and Days Since The Last Doctor Visit", xlab = 'Party', ylab = 'Days Since The Last Doctor Visit', col = c("green"))

mosaicplot(train$Most_Listened_To_Song ~ train$Party, main = "Political Party and The Most Listened to Song", ylab = 'Party', xlab = 'The Most Listened to Song', col = c("green"))
mosaicplot(train$Favorite_Color ~ train$Party, main = "Political Party and Favorite Color", ylab = 'Party', xlab = 'Favorite Color', col = c("Red"))
boxplot(train$Shoe_Size ~ train$Party, main = "Political Party and Shoe Size", xlab = 'Party', ylab = 'Shoe Size', col = c("green"))

tree <- rpart(Party ~ Shoe_Size + Most_Listened_To_Song + Days_Since_Last_Doctor_Visit, data = train, method = 'class')
predict(tree,train, type = 'class')
rpart.plot(tree)
 View(tree)
prp(tree, extra = 100, box.col = "green")
CrossValidation::cross_validate(train,tree,2,0.8)

tree <- rpart(Party ~ Shoe_Size + Most_Listened_To_Song + Days_Since_Last_Doctor_Visit, data = train, method = 'class', minsplit = 10, minbucket = 10)
predict(tree,train, type = 'class')
rpart.plot(tree)
View(tree)
prp(tree, extra = 100, box.col = "green")
CrossValidation::cross_validate(train,tree,5,0.8)



pred.tree <- predict(tree,newdata = test, type = "class")
test$Party <- pred.tree
FBsubmission$Party <- pred.tree
write.csv(FBsubmission, "FBsubmission.csv", row.names = FALSE)
