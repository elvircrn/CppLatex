/**
Credits
 
Elvir Crncevic

License
 
The MIT License (MIT)

Copyright (c) 2017 Elvir Crncevic

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef SIMPLEXGENERATOR_TABLE_H
#define SIMPLEXGENERATOR_TABLE_H

#include <limits>
#include <vector>
#include <iomanip>
#include <sstream>
#include <set>

#include "latex.h"

using std::vector;
using std::pair;
using std::string;
using std::set;

namespace ltx {
template<class T>
class Table {
public:
  typedef pair<pair<size_t, size_t>, pair<size_t, size_t>> Arrow;
private:
  bool showArrows = true;
  int tikzId;

  set<Arrow> arrows;
  string topLeft;
  vector<string> header;
  vector<string> sidebar;

  vector<bool> highCols;
  vector<bool> highRows;
  vector<vector<T>> mat;

  vector<vector<T>> alt;
  vector<vector<bool>> hasAlt;

  std::string get(size_t i, size_t j) const {
    if (!i && !j) {
      return topLeft;
    } else if (!i) {
      return header[j - 1];
    } else if (!j) {
      return sidebar[i - 1];
    } else {
      std::stringstream ss;
      ss << mat[i - 1][j - 1];
      return ss.str();
    }
  }

  void expand(size_t x, size_t y) {
    // Expand rows
    for (size_t i = 0; i < x; i++) {
      mat.push_back(vector<T>(width()));
      alt.push_back(vector<T>(width()));
      sidebar.emplace_back("");
      hasAlt.emplace_back(vector<bool>(width()));
      highRows.push_back(false);
    }

    _height += x;

    // Expand columns
    for (size_t i = 0; i < y; i++) {
      header.push_back("");
      highCols.push_back(false);
      size_t h = height();
      for (size_t j = 0; j < h; j++) {
        mat[j].push_back(T());
        alt[j].push_back(T());
        hasAlt[j].push_back(false);
      }
    }
    _width += y;
  }

  string getCellTikzMark(size_t i, size_t j) const {
    std::stringstream ss;
    ss << " \\tikzmark{" << std::to_string(tikzId) + "_r" << i * height() + j << "} ";
    return ss.str();
  }

  string drawArrow(const Arrow &arrow) const {
    std::stringstream ss;
    string from = std::to_string(tikzId) + "_r" + std::to_string(arrow.first.first * height() + arrow.first.second);
    string to = std::to_string(tikzId) + "_r" + std::to_string(arrow.second.first * height() + arrow.second.second);
    ss << "\\draw [->] ([yshift=.75pt]{pic cs:" << from << "}) -- ({pic cs:" << to << "});\n";
    return ss.str();
  }

  Table() {
    tikzId = rand();
    _height = 0;
    _width = 0;
  }

  class BracketProxy {
    Table<T> *table;
    size_t x;

  public:
    BracketProxy(Table<T> *_table, size_t _x) : table(_table), x(_x) { }

    T& operator[](const size_t &_y) {
      return table->mat[x][_y];
    }
  };

  size_t _height;
  size_t _width;

  size_t maxElemWidth() const {
    size_t maxw = 0;
    for (const auto &row : mat) {
      for (const auto &elem : row) {
        std::stringstream elemss;
        elemss << elem;
        maxw = std::max(maxw, elemss.str().size());
      }
    }
    return maxw;
  }

public:
  size_t height() const { return _height; }
  size_t width() const { return _width; }

  size_t fullHeight() const { return height() + 1; }
  size_t fullWidth() const { return width() + 1; }

  BracketProxy operator[](const size_t &x) {
    return BracketProxy(this, x);
  }

  void generateNewTikzId() {
    tikzId = rand();
  }

  explicit Table(const vector<vector<T>> &_mat) : Table() {
    expand(_mat.size(), _mat[0].size());
    mat = _mat;
  }

  Table(const vector<string> &_header, const vector<string> &_sidebar) : Table() {
    expand(_header.size(), _sidebar.size());
    setHeader(_header);
    setSidebar(_sidebar);
  }

  Table(const vector<vector<T>> &_mat, const vector<string> &_header, const vector<string> &_sidebar) : Table() {
    expand(_sidebar.size(), _header.size());
    mat = _mat;
    setHeader(_header);
    setSidebar(_sidebar);
  }

  Table<T> &setRow(size_t r, const vector<T> &row) {
    for (size_t i = 0; i < row.size(); i++)
      mat[r][i] = row[i];
    return *this;
  }

  Table<T> &setCol(size_t c, const vector<T> &col) {
    for (size_t i = 0; i < col.size(); i++)
      mat[i][c] = col[i];
    return *this;
  }

  Table<T> &enableArrows() {
    showArrows = true;
    return *this;
  }

  Table<T> &disableArrows() {
    showArrows = false;
    return *this;
  }

  Table<T> &setHighCols(const vector<bool> &_highCols) {
    highCols = _highCols;
    return *this;
  }

  Table<T> &setHighRows(const vector<bool> &_highRows) {
    highRows = _highRows;
    return *this;
  }

  Table<T> &setHighRow(size_t i) {
    highRows[i] = true;
    return *this;
  }

  Table<T> &setHighCol(size_t i) {
    highCols[i] = true;
    return *this;
  }

  Table<T> &removeAlt(size_t i, size_t j) {
    hasAlt[i][j] = false;
    return *this;
  }

  Table<T> &setAlt(const T &elem, size_t i, size_t j) {
    hasAlt[i][j] = true;
    alt[i][j] = elem;
    return *this;
  }

  Table<T> &setAlt(const vector<vector<T>> &_alt) {
    for (size_t i = 0; i < _alt.size(); i++) {
      for (size_t j = 0; j < _alt[i].size(); j++) {
        setAlt(_alt[i][j], i, j);
      }
    }
    return *this;
  }

  Table<T> &setSidebar(const vector<string> &_sidebar) {
    sidebar = _sidebar;
    return *this;
  }

  Table<T> &setHeader(const vector<string> &_header) {
    header = _header;
    return *this;
  }

  Table<T> &addRow(const string &side, const vector<T> &row) {
    expand(1, 0);
    for (size_t i = 0; i < row.size(); i++)
      mat.back()[i] = row[i];
    sidebar.back() = side;
    return *this;
  }

  Table<T> &addCol(const string &head, const vector<T> &col) {
    expand(0, 1);
    for (size_t i = 0; i < col.size(); i++)
      mat[i].back() = col[i];
    header.back() = head;
    return *this;
  }

  std::string rawTable() const {
    std::stringstream ss;

    int maxw = (int)maxElemWidth();

    for (const auto &row : mat) {
      for (const auto &elem : row) {
        ss << std::setw(maxw) << elem << ' ';
      }
      ss << '\n';
    }
    return ss.str();
  }

  Table<T> &addArrow(Arrow arrow) {
    arrows.insert(arrow);
    return *this;
  }

  Table<T> &remvoeArrow(Arrow arrow) {
    if (arrows.find(arrow) != arrows.end())
      arrows.erase(arrows.find(arrow));
    return *this;
  }

  std::string toLatex() const {
    std::stringstream ss;

    ss << ltx::BeginStr("tabular") << '\n';

    ss << "{ | " << (highCols[0] ? ltx::HIGH_COL : "") << " c |";
    for (size_t i = 1; i < fullWidth(); i++)
      ss << " " << (highCols[i - 1] ? ltx::HIGH_COL : "") << " c |";
    ss << "}\n";

    ss << "\\hline\n";

    for (size_t i = 0; i < fullHeight(); i++) {
      ss << ((i && highRows[i - 1]) ? HIGH_ROW : "");
      for (size_t j = 0; j < fullWidth(); j++) {
        if (i && j && hasAlt[i - 1][j - 1])
          ss << "\\slashbox{" << alt[i - 1][j - 1] << "}{" << get(i, j) << "}";
        else
          ss << get(i, j);

        if (showArrows && !arrows.empty())
          ss << getCellTikzMark(i, j);

        if (j != fullWidth() - 1)
          ss << " & ";
      }
      ss << ltx::BLANK << '\n' << ltx::HLINE << '\n';
    }

    ss << ltx::EndStr("tabular") << '\n';

    if (showArrows && !arrows.empty()) {
      ss << '\n' << ltx::BeginStr("tikzpicture")
         << "[overlay, remember picture, yshift=.25\\baselineskip, shorten >=.5pt, shorten <=.5pt]\n";

      for (const auto &arrow : arrows) {
        ss << drawArrow(arrow);
      }

      ss << ltx::EndStr("tikzpicture") << '\n';
    }

    return ss.str();
  }

  friend class BracketProxy;
};
}

#endif //SIMPLEXGENERATOR_TABLE_H
