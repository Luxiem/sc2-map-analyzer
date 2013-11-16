#include "Stack.h"
#include "RectManager.h"
#include "DrawManager.h"


namespace UV
{


Stack::Stack() : Container()
{
  
}


Stack::~Stack()
{
}


void Stack::Add(Widget* a_widget)
{
  // Set parent heirarchy
  a_widget->SetParent(this);

  Container::Add(a_widget);
}


void Stack::OnMouseMove(long a_x, long a_y)
{
  Container::OnMouseMove(a_x, a_y);
}


bool Stack::OnMousePressed(unsigned short a_x, unsigned short a_y)
{
  return Container::OnMousePressed(a_x, a_y);
}


bool Stack::OnMouseReleased(unsigned short a_x, unsigned short a_y)
{
  return Container::OnMouseReleased(a_x, a_y);
}


void Stack::Draw()
{
/*
  // Draw border
  RectManager* rm = DrawManager::GetRectManager();

  Declaration2 border;
  border.Color0 = 0xffffffff;
  border.Color1 = 0xffffffff;
  border.Color2 = 0xffafafaf;
  border.Color3 = 0xffafafaf;
  border.Fill = false;
  GetRect(border.Rect);
  rm->Draw(border);
*/

  // Draw child widgets
  Container::Draw();
}


void Stack::OnChildAdded(Widget* a_widget)
{
  Container::OnChildAdded(a_widget);
}


void Stack::OnChildRemoved(Widget* a_widget)
{
  Container::OnChildRemoved(a_widget);
}


} // namespace
