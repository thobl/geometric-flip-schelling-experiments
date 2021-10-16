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

tbl_lines <- tbl[tbl$factor <= 3.2 & tbl$factor >= 1.4, ]
tbl_labels <- tbl_lines[tbl_lines$n == 10000, ]
tbl_lines <- tbl[tbl$factor <= 2.6 & tbl$factor >= 1.4, ]

p <- ggplot(tbl,
            aes(x = deg_avg_exp,
                y = degen,
                color = factor(n))) +
    geom_line(data = tbl_lines, aes(group = factor), color = "gray") +
    geom_line() + geom_point() +
    geom_segment(data = tbl_labels, aes(xend = deg_avg_exp, yend = -Inf),
                 linetype = "dashed") +
    geom_label(data = tbl_labels,
               aes(label = paste0("$", factor, "\\sqrt{n}$"),
                   ## y = 0.5 * degen - 0.01,
                   y = (factor - 1.4) / 0.3 / 15),
               show.legend = FALSE, size = 3.1) +
    scale_color_manual(values = colors) +
    xlab("expected average degree") +
    ylab(paste("probability to become monocolored\n(after",
               max_iteration, "iterations)")) +
    scale_color_manual(name = "n", values = colors)
ggsave("pdf/crit_avg_deg.pdf", plot = p, width = 6, height = 4)

tikz(file = "tex/crit_avg_deg.tex", width = 5.5, height = 2.8)
print(p)
dev.off()
