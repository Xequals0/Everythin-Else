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
 
train <- read.csv("~/Desktop/Friend_Training_.csv", stringsAsFactors=FALSE)
   View(train)
test <- read.csv("~/Desktop/Friend_Testing.csv", stringsAsFactors=FALSE)
   View(test)
Friend_submission <- read.csv("~/Desktop/Friend_submission.csv", stringsAsFactors=FALSE)
   View(Friend_submission)   

train$Friends <- as.character(train$Friends)

boxplot(train$Years_Known~ train$Friends, main = "Friendship With Respect to Years Known", xlab = 'Friends', ylab = 'Years Known', col = c("green"))
boxplot(train$Duration_Of_Interaction~ train$Friends, main = "Friendship With Respect to Duration", xlab = 'Friends', ylab = 'Duration of Interaction', col = c("green"))
mosaicplot(train$Type_Of_Interaction~train$Friends, main = "Friendship With Respect to Type of Interaction", ylab = 'Friends', xlab = 'Type of Interaction', col = c("green"))
mosaicplot(train$Moon_Phase~train$Friends, main = "Friendship With Respect to Moon Phase", ylab = 'Friends', xlab = 'Moon Phase', col = c("green"))

boxplot(train$Years_Known~train$Type_Of_Interaction, main = "Years Known with Respect to Type of Interaction", xlab = 'Type of Interaction', ylab = 'Years Known', col = c("green"))
boxplot(train$Years_Known~train$Moon_Phase, main = "Years Known with Respect to Moon Phase", xlab = 'Moon Phase', ylab = 'Years Known', col = c("green"))
plot(train$Years_Known~ train$Duration_Of_Interaction, main = "Duration of Interaction with Respect to Years Known", xlab = 'Duration of Interaction', ylab = 'Years Known', col = c("green"))
boxplot(train$Duration_Of_Interaction~ train$Type_Of_Interaction, main = "Duration of Interaction with Respect to Type of Interaction", xlab = 'Type of Interaction', ylab = 'Duration of Interaction', col = c("green"))
boxplot(train$Duration_Of_Interaction~ train$Moon_Phase, main = "Duration of Interaction with Respect to Moon Phase", xlab = 'Moon Phase', ylab = 'Duration of Interaction', col = c("green"))
mosaicplot(train$Moon_Phase~train$Type_Of_Interaction, main = "Type of Interaction with Respect to Moon Phase", ylab = 'Type of Interaction', xlab = 'Moon Phase', col = c("green"))

train$total_time <- (train$Years_Known + train$Duration_Of_Interaction)
test$total_time <- (test$Years_Known + test$Duration_Of_Interaction)

tree <- rpart(Friends ~  total_time + Type_Of_Interaction + Moon_Phase, data = train, method = 'class')
predict(tree,train, type = 'class')
rpart.plot(tree)
  View(tree)
prp(tree, extra = 100, box.col = "green")
CrossValidation::cross_validate(train, tree, 2, 0.8)

pred.tree <- predict(tree,newdata = test, type = "class")
test$Friends <- pred.tree
Friend_submission$Friends <- pred.tree
write.csv(Friend_submission, "Friend_submission.csv", row.names = FALSE)
