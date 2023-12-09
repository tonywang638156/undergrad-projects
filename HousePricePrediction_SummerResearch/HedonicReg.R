x <- readRDS("~/Desktop/Spring2023/advacne_econometrix/summer_research/2nd part/data/Housing01.rds")
x <- x[x$SaleAmt >= 50000,] 

# regular
fit.reg <- lm(log(SaleAmt) ~ Med.Income  + LivingSqFt + LtArea + + Educ_High +
                Perc_18 + Dist_cbd + Park_Pct_5 + Park_Pct_10 + Price.Annual + factor(Garage) + factor(YearSold) + 
                factor(tract) + factor(TotalBath) + factor(BedroomsNbr) + factor(RoomsNbr), data = x)
summary(fit.reg)

# panal 
library(plm)
fit.panal <- plm(log(SaleAmt) ~ Med.Income, index = c('tract', 'YearSold'), model = 'within', effect = 'twoways', data = x)
summary(fit.panal)

# splines
library(splines)
q <- quantile(x$Med.Income, c(0.2,0.4,0.6,0.8))
fit.spline <- lm(log(SaleAmt) ~ factor(YearSold) + bs(Med.Income, knots = q, degree = 2), data = x)
summary(fit.spline)

