#ifndef VERTEX_TABLE_HPP
#define VERTEX_TABLE_HPP

#include <QDialog>
#include <QHBoxLayout>
#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QStringList>
#include <iostream>
#include <vector>
#include "Wed.hpp"


#define COLUMN_WIDTH 100
#define NUM_COLS_WED 9
#define NUM_COLS_VERT 1
#define NUM_COLS_FACE 1


class VertexTable : public QWidget
{
  Q_OBJECT
public:
  VertexTable();
  ~VertexTable();
  void makeVertexTable(std::vector<Vertex*> * vertexes);
  QTableWidget * getVertexTable();
private:
  QTableWidget * vertexTable;
  QStringList vertexLabels;
  int currentIndex;
  std::string currentVal;


public slots:
  void popupContextMenuVertex(QPoint pos);
};




#endif // VERTEX_TABLE_HPP