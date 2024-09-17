#pragma once

#include "imgui.h"

class ControlPanel {
  protected:
    const int width = 300;
    const int height = 300;

  private:
    // GUI
    void initFont();
    void styleWidget();

  public:
    ControlPanel();

    void setup();
    void run();
};
