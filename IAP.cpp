#include "IAP.h"
#include <iostream>

void IAP::run ()
{
    //Instructions
    std::cout << "Hello! Welcome to the 'just okay' polyphonic synth\n\nInstructions:\n\n";
    std::cout << "Slider Control:\n41 - Volume Control\n42 - LPF Control\n43 - HPF Control\n\n";
    std::cout << "Wavetype Selection:\n51 - Sine\n52 - Square/Pulse\n53 - Saw\n54 - Triangle\n\n";
    std::cout << "Recording a sequence:\n- Press record button(117).\n- Play sequence and then press stop button(114).\n- Press play button(115) to play back sequence.\n- To clear sequence, press the clear button(116).\n- Buttons 55 to 58 control interval between notes in sequence: Maj3, Min3, Fifth, Octave.\n\n";
    std::cout << "Rotary Knobs:\n21 - Sequence Playback Speed Control\n25 - Panning Control.\n\n";
    std::cout << "Note:\n-Sequencer plays on osc 22 and 23\n-Pan only affects sequence\n\n";
    std::cout << "Press 59 to exit program\n";
    //Main
    for(int i = 0; i < oscsAndNotes.size(); i++)
    {
        oscsAndNotes[i] = -1;
    }
    while(exit)
        {
            if(playBtn == true)
            {
                for ( int i = 0; i < noteList.size(); i++)
                {
                    aserveOscillator(22, mtof(noteList[i].osc0), volumeSlider, waveType);
                    aserveOscillator(23, mtof(noteList[i].osc0+interval), volumeSlider, waveType);
                    aserveSleep(playbackSpeed);
                }
            }
            else
            {
                aserveSleep(100);
            }
        }
}

void IAP::callbackNoteReceived(int note, int velocity, int channel)
{
    if(velocity > 0 )
    {
        for (int i = 0; i < oscsAndNotes.size(); i++)
        {
            if(oscsAndNotes[i] == -1)
            {
                oscsAndNotes[i] = note;
                aserveOscillator(i, mtof(note), volumeSlider, waveType);
                break;
            }
        }
        if(recBtn == true)
        {
            oscillators newNotes;
            newNotes.osc0 = note;
            noteList.push_back(newNotes);
        }
    }
    else
    {
        for (int i = 0; i < oscsAndNotes.size(); i++)
        {
            if(oscsAndNotes[i] == note)
            {
                aserveClearOscillator(i);
                oscsAndNotes[i] = -1;
                break;
            }
        }
    }
}

void IAP::callbackCCValueChanged(int cc, int value)
{
    enum eControls
    {
        eStop = 114, ePlay = 115, eRecord = 117, eClear = 116,
        eMaj3 = 55, eMin3 = 56, eFifth = 57, eOctave = 58,
        eSine = 51, eSquare = 52, eSaw = 53, eTriangle = 54,
        eVolume = 41, eLPF = 42, eHPF = 43,
        ePan = 25, ePlayBackSpeed = 21,
        eEXIT = 59,
    };
    //Recording and Playback
    if(cc == eStop)
    {
        playBtn = false;
        recBtn = false;
        aserveClearOscillator(22);
        aserveClearOscillator(23);
    }
    else if(cc == ePlay )
    {
        playBtn = true;
    }
    else if(cc == eRecord)
    {
        recBtn = true;
    }
    else if(cc == eClear)
    {
        noteList.clear();
    }
    //Intervals
    else if (cc == eMaj3)
    {
        interval = 4;
    }
    else if (cc == eMin3)
    {
        interval = 3;
    }
    else if (cc == eFifth)
    {
        interval = 7;
    }
    else if (cc == eOctave)
    {
        interval = 12;
    }
    //Wavetype
    else if(cc == eSine)
    {
        waveType = 0;
    }
    else if(cc == eSquare)
    {
        waveType = 1;
    }
    else if(cc == eSaw)
    {
        waveType = 2;
    }
    else if(cc == eTriangle)
    {
        waveType = 4;
    }
    //Sliders
    else if(cc == eVolume)
    {
        volumeSlider = pow(value / 127.0, 3.0);
    }
    else if(cc == eLPF)
    {
        float fLog = pow(value / 127.0, 3.0);
        float cutoff = fLog * 18000 + 20;
        aserveLPF(cutoff);
    }
    else if(cc == eHPF)
    {
        float fLog = pow(value / 127.0, 3.0);
        float cutoff = fLog * 18000 + 20;
        aserveHPF(cutoff);
    }
    //Knobs
    else if(cc == ePan)
    {
        float right = value/127.0;
        float left = 1.0 - right;
        aservePanOscillator(22, left, right);
        aservePanOscillator(23, left, right);
    }
    else if(cc == ePlayBackSpeed)
    {
        playbackSpeed = value*9.45;
    }
    
    else if(cc == eEXIT)
    {
        exit = false;
    }
    
}

float IAP::mtof(int note)
{
    const float frequency = 440.0*pow(2.0,(note-69)/12.0);
    return frequency;
}

