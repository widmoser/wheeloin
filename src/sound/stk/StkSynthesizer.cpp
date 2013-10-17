//
//  StkSynthesizer.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/17/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "StkSynthesizer.h"

StkSynthesizer::StkSynthesizer(std::string rawwavePath, int channels, double sampleRate) : channels(channels), voicer(0.0), voiceCount(0) {
    Stk::setSampleRate(sampleRate);
    Stk::setRawwavePath(rawwavePath);
    
    RtAudio::StreamParameters parameters;
    parameters.deviceId = dac.getDefaultOutputDevice();
    parameters.nChannels = channels;
    RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
    unsigned int bufferFrames = RT_BUFFER_SIZE;
    dac.openStream( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)this );
}

StkSynthesizer::~StkSynthesizer() {
    dac.closeStream();
}

void StkSynthesizer::addInstrument(Instrmnt* instrument) {
    voicer.addInstrument(instrument, voiceCount++);
    activeNoteId.push_back(-1);
    activeNote.push_back(-1);
}

int StkSynthesizer::getVoiceCount() {
    return voiceCount;
}

void StkSynthesizer::noteOn(double note, double amplitude, int voice) {
    activeNoteId[voice] = voicer.noteOn(note, amplitude, voice);
    activeNote[voice] = note;
}

void StkSynthesizer::noteOff(int voice) {
    voicer.noteOff(activeNoteId[voice], 0.0);
    activeNoteId[voice] = -1;
    activeNote[voice] = -1;
}

bool StkSynthesizer::isNoteOn(int voice) {
    return activeNoteId[voice] >= 0;
}

double StkSynthesizer::getNote(int voice) {
    return activeNote[voice];
}

void StkSynthesizer::start() {
    dac.startStream();
}

void StkSynthesizer::stop() {
    dac.stopStream();
}

/*
 Output sound 
 */
int StkSynthesizer::tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
    StkSynthesizer* data = (StkSynthesizer*) dataPointer;
    register StkFloat sample;
    register StkFloat *samples = (StkFloat *) outputBuffer;
    
    for (unsigned int i = 0; i < nBufferFrames; i++) {
        sample = data->effect.tick(data->voicer.tick());
        for (int k=0; k < data->channels; ++k)
            *samples++ = sample;
    }
    
    return 0;
}