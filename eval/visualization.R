library(ggplot2)
library(tikzDevice)

source("general.R")

tbl <- read.csv("data/visualization.csv")
tbl <- add_fractions(tbl)
p <- ggplot(tbl[tbl$iteration <= 6, ],
               aes(x = iteration,
                   y = monochrome_fraction)) +
    geom_line() +
    geom_point() +
    xlab("iteration") +
    ylab("fraction of monochrome edges") +
    theme(axis.title.y = element_blank())
ggsave("pdf/visualization-plot.pdf", plot = p, width = 6, height = 2)

tikz(file = "tex/visualization-plot.tex", width = 4.8, height = 1.0)
print(p)
dev.off()
