 Athlete_GPA_Final <- read.csv("~/Downloads/Athlete_GPA_Final.csv")
   View(Athlete_GPA_Final)
   gpa <- Athlete_GPA_Final
   View(gpa)

athlete <- subset(gpa, gpa$Athletic_Ability > 5.0)
non_athlete <- subset(gpa, gpa$Athletic_Ability < 5.0)

gpa[,7] <- ""
 colnames(gpa)[7] <- "Athlete"
 gpa[gpa$Athletic_Ability > 5.0, 7] = "athlete"
 gpa[gpa$Athletic_Ability < 5.0, 7] = "non-athlete"
 
boxplot(gpa$GPA~gpa$Athlete, main = "Student GPA Among Athletes and Non-athletes", ylab = "GPA",xlab = "Student Categories", pch = 3, col = c("Red","Green"), varwidth = TRUE)

plot(athlete$Hours_Spent_On_Homework_Per_Week,athlete$GPA, ylab = "GPA",xlab = "Time Spent on Homework (hrs)")
title(main = "Student GPA Among Athletes and Non-athletes",col.main = "Black")
abline(lm(athlete$GPA~athlete$Hours_Spent_On_Homework_Per_Week),col="Orange")

athUnivMed <- tapply(athlete$GPA, athlete$School,median)
table(athUnivMed)
nonathUnivMed <- tapply(non_athlete$GPA, non_athlete$School,median)
table(nonathUnivMed)

barplot(athUnivMed, main="GPA vs. School Among Athletes", col.main="Black",xlab = "University",ylab = "GPA",col = c("lavender", "cornsilk"), col.lab="Black")
barplot(nonathUnivMed, main="GPA vs. School Among Non-Athletes", col.main="Black",xlab = "University",ylab = "GPA",col = c("Red", "pink"), col.lab="Black")

boxplot(athlete$GPA~athlete$Major, main = "Student GPA vs. Major Among Athletes", ylab = "GPA",xlab = "Major", pch = 3, col = c("Blue","Cyan"), varwidth = TRUE)

mean.ath <- mean(athlete$GPA)
 mean.nath <- mean(non_athlete$GPA)
 sd.ath <- sd(athlete$GPA)
 sd.nath <- sd(non_athlete$GPA)
 len_ath <- length(athlete$GPA)
 len_nath <- length(non_athlete$GPA)
 sd.ath.nath <- sqrt(sd.ath^2/len_nath + sd.nath^2/len_ath)
 zeta <- (mean.ath - mean.nath)/sd.ath.nath
 zeta
6.125833