WALMART1 <- read.csv("~/Downloads/WALMART1.csv")
  View(WALMART1)
  wal <- WALMART1
  
install.packages("devtools")
devtools::install_github("devanshagr/PermutationTestSecond")
devtools::install_github("devanshagr/PermutationTestManual")

PermutationTestSecond::Permutation(wal,"COOKIES","TEA",1000,"1","0")
PermutationTestSecond::Permutation(wal,"COOKIES","SODA",1000,"1","0")
  table(wal$SODA,wal$COOKIES)
PermutationTestSecond::Permutation(wal,"COOKIES","COFFEE",1000,"1","0")
PermutationTestSecond::Permutation(wal,"SALT","FISH",1000,"1","0")
PermutationTestSecond::Permutation(wal,"SALT","STEAK",1000,"1","0")
  table(wal$SALT,wal$STEAK)
PermutationTestSecond::Permutation(wal,"FISH","SODA",1000,"1","0")
  table(wal$FISH,wal$SODA)
