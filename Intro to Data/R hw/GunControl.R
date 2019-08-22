 Gun_Control <- read.csv("~/Downloads/Gun_Control.csv")
   View(Gun_Control)

strict <- subset(Gun_Control, Gun_Laws == "Strict_Gun_Laws")
loose <- subset(Gun_Control, Gun_Laws == "Loose_Gun_Laws")

boxplot(strict$Monetary_Damage ~ strict$Weapon_Used,main='Monetary Damage by The Weapon Used Under Strict Gun Laws',col.main = "Green", xlab='Weapon Used',ylab='Monetary Damage', col.lab="Blue",col=c("Purple","Lavender"),varwidth=TRUE,pch=3)
boxplot(loose$Monetary_Damage ~ loose$Weapon_Used,main='Monetary Damage by The Weapon Used Under Loose Gun Laws',col.main = "Green", xlab='Weapon Used',ylab='Monetary Damage', col.lab="Blue",col="Cyan",varwidth=TRUE,pch=1)

strictDamMed <- tapply(strict$Monetary_Damage, strict$Weapon_Used,median)
table(strictDamMed)
looseDamMed <- tapply(loose$Monetary_Damage, loose$Weapon_Used,median)
table(looseDamMed)

strictMoonMean <- tapply(strict$Monetary_Damage, strict$Moon_Phase,mean)
barplot(strictMoonMean,main="Monetary Damage vs. Moon Phase Under Strict Gun Laws", col.main="red",xlab = "Moon Phase",ylab = "Mean Monetary Damage",col = c("lightblue", "mistyrose", "lightcyan", "lavender", "cornsilk"),col.lab="Green")
looseMoonMean <- tapply(loose$Monetary_Damage, loose$Moon_Phase,mean)
barplot(looseMoonMean,main="Monetary Damage vs. Moon Phase Under Loose Gun Laws", col.main="red",xlab = "Moon Phase",ylab = "Mean Monetary Damage",col = c("lightblue", "mistyrose", "lightcyan", "lavender", "cornsilk"),col.lab="Blue")

strictWeatherMean <- tapply(strict$Monetary_Damage, strict$Weather,mean)
barplot(strictWeatherMean,main="Monetary Damage vs. The Weather Under Strict Gun Laws", col.main="red",xlab = "The Weather",ylab = "Mean Monetary Damage",col = c("lightblue", "mistyrose", "lightcyan", "lavender", "cornsilk"),col.lab="Blue")
looseWeatherMean <- tapply(loose$Monetary_Damage, loose$Weather,mean)
barplot(looseWeatherMean,main="Monetary Damage vs. The Weather Under Loose Gun Laws", col.main="red",xlab = "The Weather",ylab = "Mean Monetary Damage",col = c("lightblue", "mistyrose", "lightcyan", "lavender", "cornsilk"),col.lab="Blue")
