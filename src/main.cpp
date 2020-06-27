#include <nanogui/nanogui.h>
#include "controlledslider.hpp"
#include "controller.hpp"
#include "store.hpp"
#include <iostream>
#include <chrono>
#include <thread>

using namespace nanogui;

class App : public Screen
{
private:
    Store *store;

public:
    MidiController *ctrl;
    bool isConnectedToTetra = false;
    bool debug;

    // NanoGUI presents many options for you to utilize at your discretion.
    // See include/nanogui/screen.h for what all of these represent.
    // screen = new Screen(Vector2i(1280, 800), "Tetra Ctrl [GL 4.1]",
    //                     /*resizable*/ true, /*fullscreen*/ false, /*colorBits*/ 8,
    //                     /*alphaBits*/ 8, /*depthBits*/ 24, /*stencilBits*/ 8,
    //                     /*nSamples*/ 0, /*glMajor*/ 4, /*glMinor*/ 1);
    App(bool debug = false) : Screen(Vector2i(1024, 740), "Dave Smith Instruments Tetr4 Controller", true, false, 8, 8, 24, 8, 0, 4, 1)
    {
        debug = debug;
        store = new Store();
        ctrl = new MidiController();
    }

    void render()
    {
        // setLayout(new GridLayout(Orientation::Vertical, 4, Alignment::Middle, 10, 15));
        // setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 8, 8));

        Window *globalSettings = new Window(this, "Global Settings");
        globalSettings->setPosition(Vector2i(0, 0));
        GridLayout *layout =
            new GridLayout(Orientation::Horizontal, 2,
                           Alignment::Middle, 0, 0);
        layout->setColAlignment(
            {Alignment::Maximum, Alignment::Fill});
        layout->setSpacing(0, 10);
        globalSettings->setLayout(layout);
        globalSettings->setFixedWidth(500);

        new Label(globalSettings, "Midi output:", "sans");
        ComboBox *midiOutSelection = new ComboBox(globalSettings, ctrl->portNames);
        midiOutSelection->setCallback([&](int newSelected) {
            ctrl->midiOut->closePort();
            std::cout << "Opening new port: " << newSelected << std::endl;
            ctrl->midiOut->openPort(newSelected);

            // Assuming correct port to Tetra is chosen for now
            isConnectedToTetra = true;
        });

        new Label(globalSettings, "Midi input:", "sans");
        ComboBox *midiInSelection = new ComboBox(globalSettings, ctrl->portNames);
        midiInSelection->setCallback([&](int newSelected) {
            ctrl->midiIn->closePort();
            ctrl->midiIn->cancelCallback();
            ctrl->midiIn->openPort(newSelected);
            ctrl->midiIn->setCallback(ctrl->callback, store);
            ctrl->midiIn->ignoreTypes(false, false, false);
        });

        // Widget *controls = new Widget(screen);
        // Screen *controls = screen;
        // controls->setLayout(new GridLayout(Orientation::Horizontal, 3, Alignment::Middle, 0, 0));

        Window *oscillatorSettings = new Window(this, "Oscillator 1");
        oscillatorSettings->setPosition(Vector2i(0, 100));
        GridLayout *oscLayout =
            new GridLayout(Orientation::Horizontal, 3,
                           Alignment::Middle, 15, 15);
        oscillatorSettings->setPosition(Vector2i{0, 100});
        oscillatorSettings->setFixedWidth(500);
        oscillatorSettings->setLayout(oscLayout);

        ControlledSlider oscFreqSlider = ControlledSlider(oscillatorSettings, ctrl, store, 20, "Osc 1 Frequency");
        ControlledSlider oscFineSlider = ControlledSlider(oscillatorSettings, ctrl, store, 21, "Osc 1 Finetune");
        ControlledSlider oscShapeSlider = ControlledSlider(oscillatorSettings, ctrl, store, 22, "Osc 1 Shape");
        ControlledSlider oscGlideSlider = ControlledSlider(oscillatorSettings, ctrl, store, 23, "Osc 1 Glide");

        Window *oscillatorSettings2 = new Window(this, "Oscillator 2");
        oscillatorSettings2->setPosition(Vector2i(0, 300));
        oscillatorSettings2->setLayout(oscLayout);
        oscillatorSettings2->setFixedWidth(500);

        ControlledSlider oscFreqSlider2 = ControlledSlider(oscillatorSettings2, ctrl, store, 24, "Osc 2 Frequency");
        ControlledSlider oscFineSlider2 = ControlledSlider(oscillatorSettings2, ctrl, store, 25, "Osc 2 Finetune");
        ControlledSlider oscShapeSlide2 = ControlledSlider(oscillatorSettings2, ctrl, store, 26, "Osc 2 Shape");
        ControlledSlider oscGlideSlide2 = ControlledSlider(oscillatorSettings2, ctrl, store, 27, "Osc 2 Glide");


        Window *subOscSettings = new Window(this, "Sub Oscillators");
        subOscSettings->setPosition(Vector2i{0, 500});
        subOscSettings->setFixedWidth(500);
        subOscSettings->setLayout(oscLayout);

        ControlledSlider subOscSlider = ControlledSlider(subOscSettings, ctrl, store, 30, "Sub Osc 1");
        ControlledSlider subOscSlider2 = ControlledSlider(subOscSettings, ctrl, store, 31, "Sub Osc 2");

        Window *mixSettings = new Window(this, "Mix");
        mixSettings->setPosition(Vector2i{0, 620});
        mixSettings->setFixedWidth(500);
        mixSettings->setLayout(oscLayout);

        ControlledSlider oscMixSlider = ControlledSlider(mixSettings, ctrl, store, 28, "Osc Mix");
        ControlledSlider oscNoiseSlider = ControlledSlider(mixSettings, ctrl, store, 29, "Noise Level");

        Window *filterSettings = new Window(this, "Filter");
        filterSettings->setPosition(Vector2i{508, 100});
        filterSettings->setFixedWidth(500);
        filterSettings->setLayout(oscLayout);

        ControlledSlider filterFreqSlider = ControlledSlider(filterSettings, ctrl, store, 102, "Filter Frequency");
        ControlledSlider filterResSlider = ControlledSlider(filterSettings, ctrl, store, 103, "Resonance");
        ControlledSlider filterKeyAmtSlider = ControlledSlider(filterSettings, ctrl, store, 104, "Filter Key Amount");
        ControlledSlider filterAudioModSlider = ControlledSlider(filterSettings, ctrl, store, 105, "Filter Audio Mod");
        ControlledSlider filterEnvAmtSlider = ControlledSlider(filterSettings, ctrl, store, 106, "Filter Envelope Amount");
        ControlledSlider filterEnvVelSlider = ControlledSlider(filterSettings, ctrl, store, 107, "Filter Envelope Velocity");
        ControlledSlider filterDelSlider = ControlledSlider(filterSettings, ctrl, store, 108, "Filter Delay");
        ControlledSlider filterAttSlider = ControlledSlider(filterSettings, ctrl, store, 109, "Filter Attack");
        ControlledSlider filterDecSlider = ControlledSlider(filterSettings, ctrl, store, 110, "Filter Decay");
        ControlledSlider filterSusSlider = ControlledSlider(filterSettings, ctrl, store, 111, "Filter Sustain");
        ControlledSlider filterRelSlider = ControlledSlider(filterSettings, ctrl, store, 112, "Filter Release");

        Window *vcaSettings = new Window(this, "VCA");
        vcaSettings->setPosition(Vector2i{508, 300});
        vcaSettings->setFixedWidth(500);
        vcaSettings->setLayout(oscLayout);

        ControlledSlider vcaLevelSlider = ControlledSlider(vcaSettings, ctrl, store, 113, "VCA Level");
        ControlledSlider vcaPanSlider = ControlledSlider(vcaSettings, ctrl, store, 114, "Output Pan");
        ControlledSlider vcaEnvAmtSlider = ControlledSlider(vcaSettings, ctrl, store, 115, "VCA Envelope Amount");
        ControlledSlider vcaEnvVelSlider = ControlledSlider(vcaSettings, ctrl, store, 116, "VCA Envelope Velocity");
        ControlledSlider vcaDelSlider = ControlledSlider(vcaSettings, ctrl, store, 117, "VCA Delay");
        ControlledSlider vcaAtkSlider = ControlledSlider(vcaSettings, ctrl, store, 118, "VCA Attack");
        ControlledSlider vcaDecaySlider = ControlledSlider(vcaSettings, ctrl, store, 119, "VCA Decay");
        ControlledSlider vcaSusSlider= ControlledSlider(vcaSettings, ctrl, store, 75, "VCA Sustain");
        ControlledSlider vcaRelSlider = ControlledSlider(vcaSettings, ctrl, store, 76, "VCA Release");

        Window *envSettings = new Window(this, "Env3");
        envSettings->setPosition(Vector2i{508, 500});
        envSettings->setFixedWidth(500);
        envSettings->setLayout(oscLayout);

        ControlledSlider envDestSlider = ControlledSlider(envSettings, ctrl, store, 85, "Env3 Destination");

        setVisible(true);
        performLayout();

        nanogui::mainloop();
    }
};

int main()
{
    nanogui::init();
    {
        App app = App(true);

        app.render();
    }
    nanogui::shutdown();
    return 0;
}
