#ifndef __IAPProjectDevelopmentTest1__IAP__
#define __IAPProjectDevelopmentTest1__IAP__


#include "AserveComs.h"

//---------------------------------------------------------------------------------
// USER CREATED CLASSES

class IAP : public AserveComs  {
public:
    
    //---------------------------------------------------------------------------------
    // SHARED VARIABLES
    typedef struct
    {
        int osc0;
    } oscillators;
    
    std::vector<oscillators> noteList;
    std::array<int, 24> oscsAndNotes;
    
    bool playBtn = false;
    bool recBtn = false;
    bool exit = true;
    float playbackSpeed = 300;
    float volumeSlider = 0.6;
    int waveType = 0;
    int currentNote = -1;
    int interval = 3;
    
    //---------------------------------------------------------------------------------
    // FUNCTIONS
    void run ();
    float mtof(int note);
    //---------------------------------------------------------------------------------
    // CALLBACK FUNCTIONS
    
    void callbackNoteReceived  (int note, int velocity, int channel);
    //void callbackModWheelMoved (int value);
    //void callbackPitchbendWheelMoved (int value);
    void callbackCCValueChanged (int cc, int value);
    //void callbackMIDIRecived (MIDI message);
    //void callbackPixelGrid (int x, int y);
private:
    
};

#endif /* defined(__IAPProjectDevelopmentTest1__IAP__) */
