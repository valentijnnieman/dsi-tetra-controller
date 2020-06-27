#pragma once
#include <nanogui/nanogui.h>
#include "controller.hpp"
#include "store.hpp"
#include <string>

using namespace nanogui;

class ControlledSlider : public Observer
{
private:
  Window *window;
  MidiController *ctrl;
  Store *store;

  Slider *slider;
  TextBox *textBox;

  unsigned int key;

public:
  ControlledSlider(Window *window, MidiController *ctrl, Store *store, unsigned int key, std::string label)
      : window(window), ctrl(ctrl), key(key), store(store)
  {
    new Label(window, label);
    slider = new Slider(window);
    slider->setValue(0.0f);
    slider->setRange({0, 127});
    slider->setFixedWidth(200);

    store->create(key, 0);
    store->attach(this);

    textBox = new TextBox(window);
    textBox->setFixedSize(Vector2i(60, 25));
    textBox->setValue("0");
    slider->setCallback([&](float value) {
      std::vector<unsigned char> message = {0, 0, 0};
      textBox->setValue(std::to_string((int)(value)));

      // Control Change: 176, 30, value
      std::cout << "Sending [176, " << this->key << ", " << (int)(value) << "]" << std::endl;
      message[0] = 176;
      message[1] = this->key;
      message[2] = (int)(value);
      this->store->update(this->key, (int)(value));
      try
      {
        this->ctrl->midiOut->sendMessage(&message);
      }
      catch (RtMidiError &error)
      {
        error.printMessage();
      }
    });
    textBox->setFixedSize(Vector2i(60, 25));
    textBox->setFontSize(20);
    textBox->setAlignment(TextBox::Alignment::Right);
  }

  void notify()
  {
    slider->setValue((float)(store->read(key)));
    textBox->setValue(std::to_string(store->read(key)));
  }
};