moody2018.a <- read.csv("~/Downloads/moody2018-a.csv")
   View(moody2018.a)
boxplot(moody2018.a$SCORE ~ moody2018.a$GRADE,main='Distribution of Student Grade by Score',ylab='Score',xlab='Letter Grade',notch=TRUE,col.lab="Purple",col=c("Blue","Cyan"),varwidth=TRUE,pch=2)

g1 <- subset(moody2018.a,moody2018.a$GRADE=="A"|moody2018.a$GRADE=="F")
g2 <- subset(g1,g1$DOZES_OFF=="always"|g1$DOZES_OFF=="never")
awake <- factor(g2$DOZES_OFF)
grade <- factor(g2$GRADE)
mosaicplot(table(grade,awake),main='The Distribution of Student Grades by Attentiveness',xlab='Letter Grade',ylab='Asleep in Class',col=c("Green","Red"))

g2 <- subset(g1,g1$TEXTING_IN_CLASS=="always"|g1$TEXTING_IN_CLASS=="never")
texting <- factor(g2$TEXTING_IN_CLASS)
grade <- factor(g2$GRADE)
mosaicplot(table(grade,texting),main='The Distribution of Student Grades by Texting Frequency',xlab='Letter Grade',ylab='Texting in Class',col=c("Green","Blue"))

boxplot(moody2018.a$PARTICIPATION ~ moody2018.a$GRADE,main='Distribution of Student Grade by Class Participation',ylab='Paticipation Index (0-1)',xlab='Letter Grade',notch=TRUE,col.lab="Blue",col=c("Red","Magenta"),varwidth=TRUE,pch=7,outline=FALSE)
