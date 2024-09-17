#pragma once

#include "Common/util.hpp"
#include "imgui.h"

class ControlPanel {
  protected:
    const int width = 300;
    const int height = 300;

  private:
    ObjectType objectType;

    // GUI
    void initFont();
    void styleWidget();

  public:
    ControlPanel();

    ObjectType getObjectType() { return objectType; }

    void setup();
    void run();
};
