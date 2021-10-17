library(ggplot2)
library(egg)
library(tikzDevice)

source("general.R")

tbl <- read.csv("data/extended.csv")
tbl <- add_fractions(tbl)
tbl <- tbl[tbl$iteration <= 25, ]

p1 <- main_plot(tbl,
                aes(x = iteration,
                    y = monochrome_fraction,
                    color = factor(deg_avg_exp))) +
    ylab("fraction of monochrome edges") +
    scale_color_manual(name = "average\ndegree", values = colors)

p2 <- main_plot(tbl,
                aes(x = iteration,
                    y = minority_count_fraction,
                    color = factor(deg_avg_exp))) +
    ylab("fraction of vertices\nbelonging to the minority") +
    scale_color_manual(name = "average\ndegree", values = colors)

p3 <- main_plot(tbl[tbl$iteration > 0, ],
                aes(x = iteration,
                    y = color_changes_fraction,
                    color = factor(deg_avg_exp))) +
    ylab("fraction of vertices changing color") +
    scale_color_manual(name = "average\ndegree", values = colors)

p <- ggarrange(p1, p2, p3, nrow = 3, ncol = 1)

ggsave("pdf/extended-plot.pdf", plot = p, width = 9, height = 9)

tikz(file = "tex/extended-plot.tex", width = 5.5, height = 6.5)
print(p)
dev.off()
