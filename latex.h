#ifndef SIMPLEXGENERATOR_LATEX_H
#define SIMPLEXGENERATOR_LATEX_H

#include <iostream>
#include <cassert>
#include <vector>
#include <sstream>

namespace ltx {
    const std::string HIGH_COL = ">{\\columncolor[gray]{0.8}}";
    const std::string HIGH_ROW = "\\rowcolor{lightgray}";
    const std::string HLINE = "\\hline";
    const std::string BLANK = "\\\\";

    void Begin(std::string s);
    void End(std::string s);
    std::string BeginStr(std::string s);
    std::string EndStr(std::string s);

    const std::string HEADER = R"(\documentclass[12pt, a4paper]{article}
\usepackage[utf8x]{inputenc}
\usepackage{amsmath}
\usepackage{tikz}
\usetikzlibrary{tikzmark}
\usepackage{slashbox}
\usepackage{fancyhdr}
\usepackage{listings}
\usepackage{amsfonts}
\usepackage{enumitem}
\usepackage{tikz}
\usepackage{graphicx}
\usepackage{colortbl}
\usepackage{mathabx}
\newcommand{\R}{\mathbb{R}}
\usepackage[margin=1in,headheight=29.6pt]{geometry}
\DeclareMathSizes{10}{9}{8}{7}
\usepackage{natbib}
\usepackage{graphicx}
\begin{document})";

    const std::string FOOTER = "\\end{document}";

	
	
	
	
}



#endif //SIMPLEXGENERATOR_LATEX_H
