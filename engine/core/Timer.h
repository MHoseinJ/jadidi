#pragma once

class Timer {
  public:
    static void initTimer();
    static float deltaTime();
    static float frameRate();

  private:
    static int lastFrame;
};
