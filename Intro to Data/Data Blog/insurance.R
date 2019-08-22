insurance <- read.csv("~/Downloads/insurance.csv")
View(insurance)

plot(insurance$age,insurance$charges, ylab = "Medical Costs (USD)",xlab = "Age (years)")
title(main = "Insurance Costs Incurred by Age",col.main = "Black")
abline(lm(insurance$charges~insurance$age),col="Orange")

plot(insurance$bmi,insurance$charges, ylab = "Medical Costs (USD)",xlab = "BMI")
title(main = "Insurance Costs Incurred by Indivduals Relative to Their BMI",col.main = "Black")
abline(lm(insurance$charges~insurance$bmi),col="Magenta")

male <- subset(insurance, insurance$sex == "male")
female <- subset(insurance, insurance$sex == "female")

 mean.male <- mean(male$charges)
 mean.female <- mean(female$charges)
 sd.male <- sd(male$charges)
 sd.female <- sd(female$charges)
 len_male <- length(male$charges)
 len_female <- length(female$charges)
 sd.male.female <- sqrt(sd.male^2/len_male + sd.female^2/len_female)
 zeta <- (mean.male - mean.female)/sd.male.female
 1 - pnorm(zeta)

 smoker <- subset(insurance, insurance$smoker == "yes")
  non_smoker <- subset(insurance, insurance$smoker == "no")
  mean.smoker <- mean(smoker$charges)
  mean.non_smoker <- mean(non_smoker$charges)
  sd.smoker <- sd(smoker$charges)
  sd.non_smoker <- sd(non_smoker$charges)
  len_smoker <- length(smoker$charges)
 len_non_smoker <- length(non_smoker$charges)
 sd.smoker.non_smoker <- sqrt(sd.smoker^2/len_smoker + sd.non_smoker^2/len_non_smoker)
 zeta <- (mean.male - mean.female)/sd.male.female
 zeta <- (mean.smoker - mean.non_smoker)/sd.smoker.non_smoker
zeta