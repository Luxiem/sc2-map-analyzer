#pragma once


namespace UV
{
  class ToggleButton;


  class ToggleGroup
  {
  public:
    ToggleGroup();
    ~ToggleGroup();

    void Add(ToggleButton* a_button);

    void Remove(ToggleButton* a_button);

    void SetToggled(ToggleButton* a_button);

  protected:
    int m_size;
    ToggleButton** m_buttons;
  };
}
