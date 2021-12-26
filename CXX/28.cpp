/**
 * @brief
 * 避免返回handlers（包括references、指针、迭代器）指向对象内部，遵守这个条款可增加封装性
 * @file 27.cpp
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-26
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */

#include <iostream>
#include <string>

class Point {
 public:
  Point(int x, int y) : x(x), y(y) {}
  void setX(int newVal) { x = newVal; }
  void setY(int newVal) { y = newVal; }

 private:
  int x, y;
};

struct RectData {
  Point ulhc;
  Point lrhc;
};

class Rectangle {
 public:
  Point& upperLeft() const { return pData->ulhc; }
  const Point& lowerRight() const { return pData->lrhc; }

 private:
  std::shared_ptr<RectData> pData;
};

class GUIObject {};

const Rectangle boudingBox(const GUIObject& obj) {}
GUIObject* pgo;
// 错误用法
const Point* pUpperLeft = &(boudingBox(*pgo).upperLeft());
