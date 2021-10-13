library(ggplot2)
library(tikzDevice)

source("general.R")

tbl <- read.csv("data/crit_avg_deg.csv")
max_iteration <- max(tbl$iteration)
tbl <- tbl[tbl$iteration == max_iteration, ]
tbl <- add_fractions(tbl)

## head(tbl[tbl$deg_avg_exp == 128, ])
## tbl[tbl$deg_avg_exp == 128 & tbl$n == 2000, ]

## tbl <- aggregate(. ~ model + n + deg_avg_exp, data = tbl, median)

## tbl[tbl$minority_count_fraction == 0, ]

p <- main_plot(tbl,
               aes(x = deg_avg_exp,
                   ## y = monochrome_fraction,
                   y = minority_count_fraction,
                   color = factor(n)))
ggsave("pdf/crit_avg_deg.pdf", plot = p, width = 6, height = 4)

ggplot(tbl,
       aes(x = n,
           y = deg_avg_exp,
           color = minority_count_fraction
           )) +
    geom_point()

ggsave("pdf/crit_avg_deg.pdf", plot = p, width = 6, height = 9)

tikz(file = "tex/crit_avg_deg.tex", width = 4.7, height = 2.1)
print(p)
dev.off()
