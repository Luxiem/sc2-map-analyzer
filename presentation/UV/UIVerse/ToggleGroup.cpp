#include "ToggleGroup.h"
#include "ToggleButton.h"
#include <stdlib.h>


#define GROWARRAY(type, allocatedCountVar, pointerVar, amount) { allocatedCountVar += amount; pointerVar = (type*)realloc(pointerVar, allocatedCountVar * sizeof(type)); }


namespace UV
{


ToggleGroup::ToggleGroup()
{
  m_buttons = 0;
  m_size = 0;
}


ToggleGroup::~ToggleGroup()
{
  if (m_buttons)
    delete[] m_buttons;
}


void ToggleGroup::Add(ToggleButton* a_button)
{
  //realloc
  if (m_buttons)
  {
    GROWARRAY(ToggleButton*, m_size, m_buttons, 1);
    m_buttons[m_size - 1] = a_button;
  }
  else
  {
    m_buttons = (ToggleButton**)malloc(sizeof(ToggleButton*));
    m_buttons[0] = a_button;
    a_button->SetToggle(true);
    m_size = 1;
  }
}


void ToggleGroup::Remove(ToggleButton* a_button)
{
  //realloc
  //TODO - pop back...
  //GROWARRAY(ToggleButton*, m_size, m_buttons, -1);
}


void ToggleGroup::SetToggled(ToggleButton* a_button)
{
  // toggle off all buttons
  for (int i = 0; i < m_size; ++i)
  {
    m_buttons[i]->SetToggle(false);
  }

  // toggle on this button
  a_button->SetToggle(true);
}


}
