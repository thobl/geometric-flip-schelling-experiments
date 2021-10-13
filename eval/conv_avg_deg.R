library(ggplot2)
library(tikzDevice)

source("general.R")

tbl <- read.csv("data/conv_avg_deg.csv")
max_iteration <- max(tbl$iteration)
tbl <- tbl[tbl$iteration %in% c(1, 2, 3, 4, max_iteration), ]
tbl <- add_fractions(tbl)

p1 <- main_plot(tbl,
                aes(x = deg_avg_exp,
                   y = monochrome_fraction,
                   color = factor(iteration))) +
    scale_x_log10() +
    annotation_logticks(sides = "b")
p2 <- main_plot(tbl,
               aes(x = deg_avg_exp,
                   y = minority_count_fraction,
                   color = factor(iteration))) +
    scale_x_log10() +
    annotation_logticks(sides = "b")
p3 <- main_plot(tbl,
               aes(x = deg_avg_exp,
                   y = color_changes_fraction,
                   color = factor(iteration))) +
    scale_x_log10() +
    annotation_logticks(sides = "b")
p <- ggarrange(p1, p2, p3, nrow = 3, ncol = 1)
ggsave("pdf/conv_avg_deg.pdf", plot = p, width = 6, height = 9)

tikz(file = "tex/conv_avg_deg.tex", width = 4.7, height = 2.1)
print(p)
dev.off()
