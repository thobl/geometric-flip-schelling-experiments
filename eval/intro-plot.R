library(ggplot2)
library(tikzDevice)

source("general.R")

tbl <- read.csv("data/intro.csv")
tbl <- tbl[tbl$iteration == 1, ]
tbl <- add_fractions(tbl)

si_latex_labels <- function(x) {
    paste0(x / 1000, "\\,k")
}

p <- main_plot(tbl,
               aes(x = n,
                   y = monochrome_fraction,
                   color = factor(deg_avg_exp))) +
    xlab("number of vertices $n$") +
    ylab("fraction of monochrome edges") +
    scale_x_continuous(labels = si_latex_labels) +
    scale_color_manual(name = "average\ndegree", values = colors)

ggsave("pdf/intro-plot.pdf", plot = p, width = 6, height = 3)

tikz(file = "tex/intro-plot.tex", width = 4.5, height = 2.0)
print(p)
dev.off()
