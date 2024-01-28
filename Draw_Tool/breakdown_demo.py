import math

import easypyplot
import matplotlib.font_manager as fm
import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
from matplotlib.patches import Patch
from matplotlib.ticker import FormatStrFormatter

"""
    Draw a graph by 4 dimensions:
          Run on different programs(benchmarks) as group,
        in a group(program) we use different tests (like different protocols),
        in a test we can compare different methods,
        a method's score includes mutiple parts.
    
    Most graphs don't need subplots to be groups, then we can use only one graph
"""

group_list = ["group0", "group1", "group2"]
test_list = ["test0", "test1", "test2"]
method_list = ["method0", "method1", "method2"]
part_dict = {"part0": "part0", "part1": "part1", "part2": "part2"}

subplot_row = 5
subplot_col = math.ceil(len(group_list) / subplot_row)

#   To place different bars automatically in graph
# in the following code x_tick is as params to be passed from functions in a group 
fig_bar_width = 4
fig_space_width = 2
fig_dims = (25, 3.5)
fig_label_fontsize = 16
ft = fm.FontProperties("Times New Roman")
ft.set_size(fig_label_fontsize)

data_folder = "data/"
fig_name = "breakdown_demo"
fig_ylims = [(0, 35)] * 5
fig_yticks = [[i for i in range(0, 31, 5)] for _ in range(5)]

# When there is a significant difference in the y-axis data, can be like this:
# fig_ylims = [(0, 0.06), (0, 3000), (0, 150), (0, 0.5), (0, 125)]
# fig_yticks = [
#     [0.000, 0.012, 0.024, 0.036, 0.048],
#     [0, 600, 1200, 1800, 2400],
#     [0, 30, 60, 90, 120],
#     [0.0, 0.1, 0.2, 0.3, 0.4],
#     [0, 25, 50, 75, 100],
# ]

origin_fig_bar_colors = ["#f38181", "#a0c0e6", "#9ee8a7"]
fig_bar_colors = []
for item in origin_fig_bar_colors:
    fig_bar_colors.append([item] * len(method_list))
# For all part in a bar, its pattern
hatches = ["\\\\\\\\\\\\", "||||||", "-----"]

pp, fig = easypyplot.pdf.plot_setup(fig_name, fig_dims)

def process_one_row(fig_ax, row, idx, all_figs, all_xticks, xtick):
    fig_xticks = []
    fig_data_2darr = []
    design_fullname = str(row[0])
    if any(design in design_fullname for design in method_list):
        row_value = []
        for col in row.keys():
            if any(part in col for part in part_dict):
                row_value.append(row[col])
        fig_data_2darr.append(row_value)
        xtick += fig_bar_width
        fig_xticks.append(xtick)
    print(fig_data_2darr)
    fig_h = easypyplot.barchart.draw(
        fig_ax,
        fig_data_2darr,
        width=fig_bar_width,
        breakdown=True,
        xticks=fig_xticks,
        colors=fig_bar_colors[idx],
        hatchs=hatches,
    )
    all_figs += [fig_h]
    all_xticks.extend(fig_xticks)
    return xtick


def process_one_csv(fig_ax, csv_path, cur_xtick, all_figs, all_xticks):
    csv_hd = pd.read_csv(csv_path, sep=",")
    xtick = cur_xtick
    for idx, row in csv_hd.iterrows():
        xtick = process_one_row(fig_ax, row, idx, all_figs, all_xticks, xtick)
    return xtick


def process_group(idx, group):
    fig_ax = fig.add_subplot(subplot_col, subplot_row, idx + 1)
    easypyplot.format.turn_off_box(fig_ax)
    cur_xtick = fig_space_width + fig_bar_width / 2
    all_figs = []
    all_xticks = []
    for test_idx, test in enumerate(test_list):
        csv_name = "group" + str(idx) + "_test" + str(test_idx) + ".csv"
        cur_xtick = process_one_csv(
            fig_ax, data_folder + csv_name, cur_xtick, all_figs, all_xticks
        )
        # bar legend to be located related to percentage of whole subplot
        fig_ax.text(
            0.18 + 0.325 * test_idx,
            -0.055,
            test,
            ha="center",
            va="center",
            fontsize=fig_label_fontsize,
            transform=fig_ax.transAxes,
        )
        cur_xtick += fig_space_width
    plt.tight_layout()
    # hratio is a param to tighten graph to be mor tight, i.e, reduce blank space outside of label area and graph area
    # hratio higher, grapher tighter
    easypyplot.format.resize_ax_box(fig_ax, hratio=0.995)

    xlabel_ft = fm.FontProperties("Times New Roman")
    xlabel_ft.set_size(fig_label_fontsize)

    fig_ax.set_title(group, multialignment="center", fontsize=fig_label_fontsize)

    """ x axis """
    fig_ax.xaxis.set_tick_params(
        pad=0
    )  # distance between x tick label and x-axis
    fig_ax.set_xlim(
        all_xticks[0] - (fig_space_width + fig_bar_width / 2),
        all_xticks[-1] + (fig_space_width + fig_bar_width / 2),
    )
    fig_ax.set_xticks(all_xticks)
    fig_ax.set_xticklabels([], fontsize=fig_label_fontsize, fontproperties=xlabel_ft)
    fig_ax.xaxis.set_ticks_position("none")  # comment for invisible x ticks

    """ y axis """
    fig_ax.yaxis.set_tick_params(
        pad=0
    )  # distance between y tick label and y-axis
    easypyplot.format.set_axis_to_percent(
        fig_ax.yaxis
    )  # Uncomment for y axis tick label in xx% format
    fig_ax.yaxis.grid(True)
    fig_ax.set_ylim(fig_ylims[idx])
    fig_ax.set_yticks(
        fig_yticks[idx],
    )

    # set tick marks format for y_axis in subplot
    fig_yticks_label = [
        "%d" % item for item in fig_yticks[idx]
    ]  

    # Change for customized y tick mrk format, just like:
    # if idx != 0 and idx != 3:
    #       fig_yticks_label = [
    #     "%d" % item for item in fig_yticks[idx]
    #     ]  
    # elif idx == 3:
    #     fig_yticks_label = ["{:.1f}".format(item) for item in fig_yticks[idx]]
    # else:
    #     fig_yticks_label = ["{:.3f}".format(item) for item in fig_yticks[idx]]
        
    fig_ax.set_yticklabels(
        fig_yticks_label, fontproperties=ft, fontsize=fig_label_fontsize
    )
    fig_ax.set_ylabel(
        "Time (s)", multialignment="center", labelpad=0.5, fontsize=fig_label_fontsize
    )


# test as subplots
for idx, test in enumerate(group_list):
    process_group(idx, test)
# distance between subplots, and their distance to most left and right
plt.subplots_adjust(hspace=0.2, wspace=0.3)
plt.subplots_adjust(bottom=0.15, left=0.05)

""" legend """
method_legend = ["method0", "method1", "method2"]
part_legend = ["part0", "part1", "part2"]
# part legend
legends = [
    Patch(label=part_legend[i], facecolor="#ffffff", hatch=hatches[i])
    for i in range(len(part_legend))
]
# test legend
legends += [
    Patch(label=method_legend[i], facecolor=origin_fig_bar_colors[i], hatch=None)
    for i in range(len(method_legend))
]
fig.legend(
    handles=legends,
    loc="lower center",
    frameon=False,
    prop={"size": fig_label_fontsize},
    bbox_to_anchor=(0.325, -0.06),
    ncol=6, # how many legend to be a row
    columnspacing=2,  # padding between columns
    labelspacing=0,  # padding between rows
)
# any modify in plt.show() will be found in the following pdf
plt.show()
fig.savefig(fig_name + ".pdf", format="pdf", bbox_inches="tight")
