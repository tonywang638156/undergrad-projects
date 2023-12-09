setwd('~/Data') # 先进到Data文件夹 set the working directory to Data folder
x <- readRDS('Wooldridge_Wages.rds')

# check the info of the dataset
dim(x) #get the dimension
names(x)#get the column name of the dataset
head(x) #quick little snap shot of the dataset
summary(x$educ) #indicating number of years of education
summary(x$IQ)

#Wage_i = B0 + B1*IQ_i + B2*Educ_i + u_i
fit <- lm(wage ~ IQ + educ, data=x) #linear model
sum.fit <- summary(fit)

#two ways to see the summary of regression
fit
sum.fit
# B1 != 5.138 B1_hat = 5.138, we can only know B1 by running the hypothesis test

qt(0.975,932) # get me the quantitle from the table; df stands for degree of freedom = n - 1 - (num of variables) =
#935 - 1 -2 = 932 sames as qt(0.025, 932)

sum.fit$coefficients
sum.fit$coefficients[2,1] + qt(0.975,932)*sum.fit$coefficients[2,2]
sum.fit$coefficients[2,1] - qt(0.975,932)*sum.fit$coefficients[2,2]

#look for all rows (Confidence Interval for all variables)
sum.fit$coefficients[,1] + qt(0.975,932)*sum.fit$coefficients[2,2]
sum.fit$coefficients[,1] - qt(0.975,932)*sum.fit$coefficients[2,2]

names(sum.fit)
sum.fit$r.squared
#R-squared (R2) is a statistical measure that represents the proportion of the variance for a dependent 
#variable that's explained by an independent variable
cor(x$IQ, sum.fit$residuals) # it's 0, why?


#####################################################################################################
x <- readRDS('Charity.rds')
head(x)
fit1 <- lm(Char.Donations ~ Income + Exp ,data = x)
fit1.sum <- summary(fit1)

#Q2
#Income         Exp 
#0.9978865   0.1086259 
test_statistic = (fit1.sum$coefficients[,1]-0)/fit1.sum$coefficients[,2]
pts = pt(test_statistic,147)
#Q3
#Income          Exp 
#2.113456e-03 8.913741e-01 
pts1 = pt(test_statistic,147, lower.tail = FALSE)
pts1
#Q4 90% CI:  Income_CI = [0.008813977, 0.032143229]; Exp_CI = [-0.044193485,0.006353125]
fit1.sum$coefficients[,1] + qt(0.95,147)*fit1.sum$coefficients[,2]
fit1.sum$coefficients[,1] - qt(0.95,147)*fit1.sum$coefficients[,2]
#Q5 95% CI:  Income_CI = [0.00655229, 0.03440492]; Exp_CI = [-0.04909380,0.01125344]
ans1 <- fit1.sum$coefficients[,1] + qt(0.975,147)*fit1.sum$coefficients[,2]
ans2 <- fit1.sum$coefficients[,1] - qt(0.975,147)*fit1.sum$coefficients[,2]
final <- c(ans1,ans2)



#####################################################################################################
x <- readRDS('~/Data/HousingIncinerator.rds')
# 1. only focus on 1981 data (price = b0 + b1*nearinc) 
#控制单一变量，fix the time variable and analyze nearinc var. only
fit1 <- lm(rprice~nearinc, data = x, subset=(y81==1))
# subset=(y81=1) is way to subset the dataset

# 2. only on 1978 data
fit2 <- lm(rprice~nearinc, data=x, subset=(y81==0))
#coefficients(fit2)
#fit2

# 3. diff-in-diff
fit3 <- lm(rprice~nearinc + y81 + nearinc*y81, data=x)
#summary(fit3)
# (-11864) <- b3 in diff-in-diff fit3 = (-30688) <-b1 in fit1 - (-18824) <-b1 in fit2

#policy effect
# 95% CI for price decrease due to incinerator proximity, we need b3 instead of b1
fit3sum <- summary(fit3)
fit3sum$coefficients[4,1] - qt(0.975,317)*fit3sum$coefficients[4,2]
fit3sum$coefficients[4,1] + qt(0.975,317)*fit3sum$coefficients[4,2]

# 4. dif-in-dif while considering other factors
fit4 <- lm(rprice~nearinc + y81 + nearinc*y81 + age + I(age^2) + rooms + baths
           + area + land + intst, data=x)
# nearinc:y81 -1.418e+04  4.987e+03  -2.843  0.00477 ** 
# more precise  coefficient of intersection since standard error went down from 7457 to 4987.27


#####################################################################################################
x <- readRDS('~/Data/Gasoline.rds')
head(x)
# get quintiles which is the knots we want for the spline
q <- quantile(x$income, c(0.2,.4,.6,.8))
x$quint <- cut(x$income,c(0,q,100000)) #cut the income and see which of these categories it fall into, break 0 to 100000 with 5 parts using q
head(x)
levels(x$quint) <- c('q1','q2','q3','q4','q5')
head(x)
# question 2 using the regular regression for each quintines
fit1 <- lm(gallons~hh.size + income, data = x, subset=(quint=='q1'))$coefficients[3] # subset=(quint=='q1') get us only x data in the first quintile (0->q1)
fit2 <- lm(gallons~hh.size + income, data = x, subset=(quint=='q2'))$coefficients[3] # [3] means we only care about the income variable
fit3 <- lm(gallons~hh.size + income, data = x, subset=(quint=='q3'))$coefficients[3] # 
fit4 <- lm(gallons~hh.size + income, data = x, subset=(quint=='q4'))$coefficients[3] # 
fit5 <- lm(gallons~hh.size + income, data = x, subset=(quint=='q5'))$coefficients[3] # 
ans2 <- c(fit1,fit2,fit3,fit4,fit5)

# question 3 using splines, use q as the thredshold
# first create the new variable (x-gamma1)+, (x-gamma2)+,(x-gamma3)+, (x-gamma4)+,(x-gamma5) , gamma_i is just the knots or the quintiles
x$k1 <- (x$income - q[1])*(x$income>q[1]) # (x$income>q[1]) is the "+": like indicator function: true if income > q1, false if income < q1
# similar to  x$k1 <- ifelse(x$income>q[1],x$income-q[1],0)
x$k2 <- (x$income - q[2])*(x$income>q[2]) 
x$k3 <- (x$income - q[3])*(x$income>q[3]) 
x$k4 <- (x$income - q[4])*(x$income>q[4]) 
fit.line <- lm(gallons ~ hh.size + income + k1 + k2 + k3 + k4, data=x)

# quadratic spline x^2
# fit.qua <- lm(gallons ~ hh.size + income + I(income^2) + k1 + k2 + k3 + k4 + I(k1^2) + I(k2^2) + I(k3^2) + I(k4^2), data=x)
# this is bad because we need to drop the linear term all (k1 + k2 + k3 + k4)
# better version
fit.qua <- lm(gallons ~ hh.size + income + I(income^2) + I(k1^2) + I(k2^2) + I(k3^2) + I(k4^2), data=x)

# cubic spline x^3
# fit.qua <- lm(gallons ~ hh.size + income + I(income^2) + I(income^3) + k1 + k2 + k3 + k4 + I(k1^2) + I(k2^2) + I(k3^2) + I(k4^2) 
# + I(k1^3) + I(k2^3) + I(k3^3) + I(k4^3) , data=x) 
# bad because we didn't drop the 1st and 2nd power term
# better version:
fit.cub <- lm(gallons ~ hh.size + income + I(income^2) + I(income^3) + I(k1^3) + I(k2^3) + I(k3^3) + I(k4^3), data=x)

library(splines)
fit.easy.poly <- lm(gallons ~ hh.size + bs(income, knots=q, degree=3), data = x) # bs() stands for b splines, degree=1 means linear spline
summary(fit.easy.poly)
# degree = 2 means quadractic , 3 means cubic
fit.easy.poly <- lm(gallons ~ hh.size + ns(income, knots=q), data = x) # ns(income, knots=q) is the natural splines
#fit1 <- lm(expenditures~age + race, data = x, subset=(quint=='q1'))$coefficients[2]
#fit2 <- lm(expenditures~age + race, data = x, subset=(quint=='q2'))$coefficients[2]
#fit3 <- lm(expenditures~age + race, data = x, subset=(quint=='q3'))$coefficients[2]
#fit4 <- lm(expenditures~age + race, data = x, subset=(quint=='q4'))$coefficients[2]
#fit5 <- lm(expenditures~age + race, data = x, subset=(quint=='q5'))$coefficients[2]
#fit6 <- lm(expenditures~age + race, data = x, subset=(quint=='q6'))$coefficients[2]
#fit7 <- lm(expenditures~age + race, data = x, subset=(quint=='q7'))$coefficients[2]
#fit8 <- lm(expenditures~age + race, data = x, subset=(quint=='q8'))$coefficients[2]
#fit9 <- lm(expenditures~age + race, data = x, subset=(quint=='q9'))$coefficients[2]
#fit10 <- lm(expenditures~age + race, data = x, subset=(quint=='q10'))$coefficients[2]
#fit11 <- lm(expenditures~age + race, data = x, subset=(quint=='q11'))$coefficients[2]
#ans_a <- c(fit1,fit2,fit3,fit4,fit5,fit6,fit7,fit8,fit9,fit10,fit11)



#####################################################################################################
x <- readRDS('~/Data/ExpendAge.rds')
# a: Cubic spline with 10 knots evenly distributed across the age distribution, 10 knots means 11 sections
q <- quantile(x$age, c(0.09,0.18,0.27,0.36,0.45,0.54,0.63,0.72,0.81,0.9))
x$quint <- cut(x$age,c(0,q,100000)) 
levels(x$quint) <- c('q1','q2','q3','q4','q5','q6','q7','q8','q9','q10','q11')
# create the 10 knots: get (x-gamma1)+
x$k1 <- (x$age - q[1])*(x$age>q[1]) 
x$k2 <- (x$age - q[2])*(x$age>q[2]) 
x$k3 <- (x$age - q[3])*(x$age>q[3]) 
x$k4 <- (x$age - q[4])*(x$age>q[4]) 
x$k5 <- (x$age - q[5])*(x$age>q[5]) 
x$k6 <- (x$age - q[6])*(x$age>q[6]) 
x$k7 <- (x$age - q[7])*(x$age>q[7]) 
x$k8 <- (x$age - q[8])*(x$age>q[8]) 
x$k9 <- (x$age - q[9])*(x$age>q[9]) 
x$k10 <- (x$age - q[10])*(x$age>q[10]) 
fit.cub.a <- lm(expenditures~age + factor(race) + I(age^2) + I(age^3) + I(k1^3) + I(k2^3) + I(k3^3) + I(k4^3) + I(k5^3)
                + I(k6^3) + I(k7^3) + I(k8^3) + I(k9^3) + I(k10^3), data=x)
# another way
fit.easy.cub.a <- lm(expenditures ~ bs(age, knots=q, degree=3) + factor(race), data = x)
# b: Cubic spline with 5 knots evenly distributed across the age distribution, 6 evenly splitted sections
qb <- quantile(x$age, c(0.166,0.332,0.5,0.664,0.83))
x$quintb <- cut(x$age,c(0,qb,100000)) 
levels(x$quintb) <- c('q1','q2','q3','q4','q5','q6')
# create the 5 knots: get (x-gamma1)+
x$k1b <- (x$age - qb[1])*(x$age>qb[1]) 
x$k2b <- (x$age - qb[2])*(x$age>qb[2]) 
x$k3b <- (x$age - qb[3])*(x$age>qb[3]) 
x$k4b <- (x$age - qb[4])*(x$age>qb[4]) 
x$k5b <- (x$age - qb[5])*(x$age>qb[5]) 
fit.cub.b <- lm(expenditures~age + factor(race) + I(age^2) + I(age^3) + I(k1b^3) + I(k2b^3) + I(k3b^3)
                + I(k4b^3) + I(k5b^3) , data=x)
# another way
fit.easy.cub.b <- lm(expenditures ~ bs(age, knots=qb, degree=3) + factor(race), data = x)
# c: Natural cubic spline with 10 knots evenly distributed across the age distribution
fit.easy.natural <- lm(expenditures ~ ns(age, knots=q) + factor(race), data = x)
# d: A quadratic (not a spline) regression: i.e. include age and age^2
fit.d <- lm(expenditures~age + I(age^2) + factor(race), data = x)


#####################################################################################################
x <- readRDS('~/Data/Gasoline.rds')

# 1. creating histogram
hist(x$gallons, breaks=20) # breaks the range of gallons into 20 evenly section # frequency can be seen as density

# 2. use a normal kernel to estimate the density of gasoline consumed, try bandwidth h = 10, 25, 50
# fg is the E(y|x): weighted average of ?????? these are just the 
fg <- density(x$gallons, bw=5, kernel = 'gaussian') # bw is the bandwidth
# Y is density, X-axix is the # of gallons
plot(fg)
# as h increase, the curve get smoother and smoother and in this graph, it gets close to normal as bandwidth increase
# the smaller the h is, the more flexible the graph looks like, there will be up and down curve
?density() # ?gives you the description of the func in R
# c("gaussian", "epanechnikov", "rectangular","triangular", "biweight","cosine", "optcosine") these are all possible kernel functions


# Nadarya-Watson Estimate: E(gasoline|income=50,000) with h = 10
h <- 10
z <- 150 # this is the point to pick to estimate which is the x_k particular point we chose and wanna estimate
k_input <- (x$income-z)/h # this is the input to our kernel function
k <- dnorm(k_input) # here our kernel function is the normal function, so use dnorm to stimulate normal func.
weighted_average = sum(k*x$gallons)/sum(k) # E(y|x) = sum[k(~)*yi]/sum[k]
weighted_average
# easier way doing this
fit.easy.NW <- ksmooth(x$income, x$gallons, kernel = 'normal', bandwidth = 10) # ks(x, y, kernel_func, h)
plot(fit.easy.NW) # this is not the density like fg, this is the weighted average of y (gallons) of everysingle point
# of x (income) in our data
# evaluate at these particular points 50, 100, 150 
pred.nw <- ksmooth(x$income, x$gallons, kernel = 'normal', bandwidth = 10, x.points = c(50,100,150)) 
pred.nw


# local linear regression: E(y|income = 50) given h = 10
h <- 10
z <- 50
k_input <- (x$income-z)/h
k <- dnorm(k_input)
w <- k # the weights is the k, which is K[(xi-x)/h]
fit <- lm(gallons ~ income, data = x, weights = w)
# easier way doing this
library(KernSmooth)
fit.ll <-locpoly(x$income, x$gallons, degree = 1, bandwidth = 10)
plot(fit.ll$x, fit.ll$y, pch='.')
new.x <- c(50,100,150)
approx(fit.ll$x, fit.ll$y, xout = new.x)
# getting the derivative graph of the local poly
fit.ll.d <-locpoly(x$income, x$gallons, degree = 1, drv = 1, bandwidth = 10, kernel = 'normal') 
# drv=1 give the estimated first derivative of particular point we chose
plot(fit.ll.d$x, fit.ll.d$y, pch='.')


#####################################################################################################

x <- readRDS('~/Data/VotingOutcomes.rds')
head(x)

# create running variable centered at 0
x$xc <- x$demvoteshare -  0.5 # give how far you above the cutoff or below the cutoff, with cutoff x* = 0.5

# subset data, focus observations around the cutoff
x1 <- subset(x, x$xc <= 0.02 & x$xc >= -0.02) # condition or (window) of the set we want: x$xc <= 0.2 & x$xc >= -0.02
x2 <- subset(x, x$xc <= 0.05 & x$xc >= -0.05) # makes the window a little bit larger

#regress on dummy: score here is the liberalness
fit1 <- lm(score ~ democrat, data=x) # run on full data set
fit2 <- lm(score ~ democrat, data=x1) # run on subset around the cutoff
fit1$coefficients
fit2$coefficients
# run y = B0 + B1*x + B2*x*T + u
# RD with running variable
fit3 <- lm(score ~ democrat + xc + democrat:xc, data = x) # ':' means '*'; democrat represents the treatment T here
fit4 <- lm(score ~ democrat + xc + democrat:xc, data = x1) # run on window
fit5 <- lm(score ~ democrat + xc + democrat:xc, data = x2) # run on larger window
fit3$coefficients
fit4$coefficients
fit5$coefficients










x <- readRDS('~/Data/TexasPrisons.rds')

# source functions
source('~/Data/Functions_SyntheticControls.R')

# create separate datasets (treated & control)
# get the Texas because it's treated
Tr <- x[which(x$state=='Texas'), ] # tells you which rows you wanna use "which(x$state=='Texas')"
Co <- x[which(x$state!='Texas'), ] # use all other sates for control

means.us <- aggregate(list('bmprate'=Co$bmprate),by=list('year'=Co$year),FUN=mean) # take the average by year
means.tx <- x.treated[,c('year','bmprate')]

#plot 
out1 <- cbind(means.tx, means.us[,2])
names(out1) <- c('year','Texas','US')

library(ggplot2)
ggplot(out1) + geom_line(aes(x=year,y=Texas), color='red') + 
  geom_line(aes(x=year,y=US), color='blue')

# create new variables
# vector of states in control group
states = unique(Co$state)
s <- length(states) 

# dataset in pre-treatment phase
Co.pre <- Co[which(Co$year < 1993),]
Tr.pre <- Tr[which(Tr$year < 1993),]

# vector of years in pre-treatment
yrs <- unique(Co.pre$year)
y <- length(yrs)

# vector of years in full analysis
yrs2 <- unique(x$year)
y2 <- length(yrs2)

# create the composite (fake Texas)
# 1. get the weights for each state: variables = income, poverty, aidscapita
# wi = 1 / (.......)
v <- c(1,0.4,0.1)
weights.calc(v) # state with higher weights means it's closer to treated group: Texas

#get optimal v
# make composite (fake Texas) look like Texas as closest as possible in pre-treatment period
v.opt <- weights.obj(c(0.5,0.5,0.5))

# use optim to generate the optimal v for each variable
g <- optim(c(0.5,0.5,0.5), weights.obj) 
# weights.obj is the function defined in Functions_SyntheticControls.R to calculate 
# sum of squared errors btw. treated and composite
V <- g$par # optimal value v for 3 variables: income, poverty, aidscapita
smallest_diff <- g$value # smallest sum of squared errors:  sum[(realY-predY)^2]

# optimal weights for each states
w<-weights.calc(V)

# calculate composite with given weigths
US.composite <- composite.calc(w) # this will generate the control dataset
out2 <- cbind(Tr[, c('year', 'bmprate','income', 'poverty', 'aidscapita')], US.composite[,-1])
out2 <- out2[,order(names(out2))]

ggplot(out2) + geom_line(aes(x=year,y=bmprate),color='red') +  # red is treated
  geom_line(aes(x=year,y=bmprate.c), color='blue') # blue is control

# compute the difference in bmprate btw Texas and SYnthetic(composite) Texas for pre- and post- treatment period
k <- which(out2$year==1992) # get the year where it starts the treatment
# difference btw treated and composite(control) in pre-treatment period
dpre <- mean(out2$bmprate[1:k]-out2$bmprate.c[1:k]) 
# difference btw treated and composite(control) after treatment 
dpost <- mean(out2$bmprate[(k+1):y2]-out2$bmprate.c[(k+1):y2])
treatment.effect <- dpost - dpre