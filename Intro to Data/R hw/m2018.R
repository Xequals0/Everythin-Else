 M2018_test_students <- read.csv("~/Downloads/M2018_test_students.csv")
   View(M2018_test_students)
 M2018_train <- read.csv("~/Downloads/M2018_train.csv")
   View(M2018_train)
 M2018_sample_submission <- read.csv("~/Downloads/M2018_sample_submission.csv")
   View(M2018_sample_submission)
 train <- M2018_train
 test <- M2018_test_students

sub_one <- subset(train, ASKS_QUESTIONS == 'always' & LEAVES_EARLY == 'never' & PARTICIPATION > 0.5)
 View(sub_one)
boxplot(sub_one$SCORE~sub_one$GRADE, main = 'Grades of Students Who Participate, Ask Questions, and Do Not Leave Early', xlab = 'Grade', ylab = 'Score', col = "RED")

sub_one <- subset(train, PARTICIPATION > 0.5)
boxplot(sub_one$SCORE~sub_one$GRADE, main = 'Grades of Students Who Participate', xlab = 'Grade', ylab = 'Score', col = "RED")

sub_one <- subset(train, PARTICIPATION < 0.5)
boxplot(sub_one$SCORE~sub_one$GRADE, main = 'Grades of Students Who Do Not Participate', xlab = 'Grade', ylab = 'Score', col = "RED")

sub_one <- subset(train, ASKS_QUESTIONS == 'never' & LEAVES_EARLY == 'always' & PARTICIPATION > 0.5)
boxplot(sub_one$SCORE~sub_one$GRADE, main = 'Grades of Students Who Participate but Do Not Ask Questions and Always Leave Early', xlab = 'Grade', ylab = 'Score', col = "RED")

sub_one <- subset(train, ASKS_QUESTIONS == 'sometimes' & PARTICIPATION > 0.5)
boxplot(sub_one$SCORE~sub_one$GRADE, main = 'Grades of Students Participate and Sometimes Ask Questions', xlab = 'Grade', ylab = 'Score', col = "RED")

sub_one <- subset(train, ASKS_QUESTIONS == 'sometimes' & PARTICIPATION < 0.5)
boxplot(sub_one$SCORE~sub_one$GRADE, main = 'Grades of Students That Do Not Participate and Sometimes Ask Questions', xlab = 'Grade', ylab = 'Score', col = "RED")

myprediction<-test
decision <- rep('F',nrow(myprediction))

 decision[myprediction$ASKS_QUESTIONS == 'always' & myprediction$LEAVES_EARLY != 'always' & myprediction$SCORE > 23] <- 'D'
 decision[myprediction$ASKS_QUESTIONS == 'always' & myprediction$LEAVES_EARLY != 'always' & myprediction$SCORE > 42] <- 'C'
 decision[myprediction$ASKS_QUESTIONS == 'always' & myprediction$LEAVES_EARLY != 'always' & myprediction$SCORE > 61] <- 'B'
 decision[myprediction$ASKS_QUESTIONS == 'always' & myprediction$LEAVES_EARLY != 'always' & myprediction$SCORE > 70] <- 'A'
 decision[myprediction$ASKS_QUESTIONS == 'never' & myprediction$LEAVES_EARLY == 'always' & myprediction$SCORE > 29] <- 'D'
 decision[myprediction$ASKS_QUESTIONS == 'never' & myprediction$LEAVES_EARLY == 'always' & myprediction$SCORE > 50] <- 'C'
 decision[myprediction$ASKS_QUESTIONS == 'never' & myprediction$LEAVES_EARLY == 'always' & myprediction$SCORE > 70] <- 'B'
 decision[myprediction$ASKS_QUESTIONS == 'never' & myprediction$LEAVES_EARLY == 'always' & myprediction$SCORE > 80] <- 'A'
 decision[myprediction$ASKS_QUESTIONS == 'sometimes' & myprediction$SCORE > 22] <- 'D'
 decision[myprediction$ASKS_QUESTIONS == 'sometimes' & myprediction$SCORE > 43] <- 'C'
 decision[myprediction$ASKS_QUESTIONS == 'sometimes' & myprediction$SCORE > 60] <- 'B'
 decision[myprediction$ASKS_QUESTIONS == 'sometimes' & myprediction$SCORE > 85] <- 'A'
 myprediction$GRADE <-decision
 test$Projection <-decision
 error <- mean(test$GRADE!= myprediction$GRADE)
 error


