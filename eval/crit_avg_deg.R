library(ggplot2)
library(tikzDevice)

source("general.R")

tbl <- read.csv("data/crit_avg_deg.csv")
max_iteration <- max(tbl$iteration)
tbl <- tbl[tbl$iteration == max_iteration, ]
tbl$degen <- tbl$minority_count == 0
tbl <- aggregate(. ~ model + n + deg_avg_exp, data = tbl, mean)

tbl$sqrtn <- sqrt(tbl$n)
tbl$factor <- tbl$deg_avg_exp / tbl$sqrtn

p <- ggplot(tbl,
            aes(x = deg_avg_exp,
                y = degen,
                color = factor(n))) +
    geom_line(data = tbl[tbl$factor <= 2.3 & tbl$factor >= 1.7, ],
              aes(group = factor), color = "gray") +
    geom_line() + geom_point() +
    scale_color_manual(values = colors) +
    geom_label(data = tbl[tbl$n == 10000 & tbl$factor <= 2.3 & tbl$factor >= 1.7, ],
               aes(label = paste("sqrt(n) *", factor)),
               color = "gray",
               nudge_x = 15, hjust = "outward", show.legend = FALSE) +
    xlab("average degree") +
    ylab("probability to become mono colored") +
    scale_color_manual(name = "n", values = colors)
ggsave("pdf/crit_avg_deg.pdf", plot = p, width = 6, height = 4)

## tikz(file = "tex/crit_avg_deg.tex", width = 4.7, height = 2.1)
## print(p)
## dev.off()
