#ifndef STK_SAMPLEINSTR_H
#define STK_SAMPLEINSTR_H

#include "Instrmnt.h"
#include "FileWave.h"

namespace stk {

/***************************************************/
/*! \class SampleInstrument
    \brief Wrapper for FileWave

    Based on VoicForm
    by Perry R. Cook and Gary P. Scavone, 1995-2012.
*/
/***************************************************/

class SampleInstrument : public Instrmnt
{
  public:
  //! Class constructor.
  /*!
    An StkError will be thrown if the rawwave path is incorrectly set.
  */
  SampleInstrument( std::string fileName );

  //! Class destructor.
  ~SampleInstrument( void );

  //! Set instrument parameters for a particular frequency.
  void setFrequency( StkFloat frequency );

  //! Set voiced component pitch sweep rate.
  void setPitchSweepRate( StkFloat rate ) { voiced_->setSweepRate(rate); };

  //! Start a note with the given frequency and amplitude.
  void noteOn( StkFloat frequency, StkFloat amplitude );

  //! Stop a note with the given amplitude (speed of decay).
  void noteOff( StkFloat amplitude ) { voiced_->noteOff(); };

  //! Perform the control change specified by \e number and \e value (0.0 - 128.0).
  void controlChange( int number, StkFloat value );

  //! Compute and return one output sample.
  StkFloat tick( unsigned int channel = 0 );

  //! Fill a channel of the StkFrames object with computed outputs.
  /*!
    The \c channel argument must be less than the number of
    channels in the StkFrames argument (the first channel is specified
    by 0).  However, range checking is only performed if _STK_DEBUG_
    is defined during compilation, in which case an out-of-range value
    will trigger an StkError exception.
  */
  StkFrames& tick( StkFrames& frames, unsigned int channel = 0 );

protected:

  FileWave *voiced_;

};

inline StkFloat SampleInstrument :: tick( unsigned int )
{
  lastFrame_[0] = voiced_->tick();
  return lastFrame_[0];
}

inline StkFrames& SampleInstrument :: tick( StkFrames& frames, unsigned int channel )
{
  unsigned int nChannels = lastFrame_.channels();
#if defined(_STK_DEBUG_)
  if ( channel > frames.channels() - nChannels ) {
    oStream_ << "VoicForm::tick(): channel and StkFrames arguments are incompatible!";
    handleError( StkError::FUNCTION_ARGUMENT );
  }
#endif

  StkFloat *samples = &frames[channel];
  unsigned int j, hop = frames.channels() - nChannels;
  if ( nChannels == 1 ) {
    for ( unsigned int i=0; i<frames.frames(); i++, samples += hop )
      *samples++ = tick();
  }
  else {
    for ( unsigned int i=0; i<frames.frames(); i++, samples += hop ) {
      *samples++ = tick();
      for ( j=1; j<nChannels; j++ )
        *samples++ = lastFrame_[j];
    }
  }

  return frames;
}

} // stk namespace

#endif
