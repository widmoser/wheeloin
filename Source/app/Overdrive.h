#ifndef STK_OVERDRIVE_H
#define STK_OVERDRIVE_H

#include "Instrmnt.h"

namespace stk {

class Overdrive : public Instrmnt
{
 public:
  //! Class constructor.
  /*!
    An StkError will be thrown if the rawwave path is incorrectly set.
  */
    Overdrive( Instrmnt* delegate ) : delegate_(delegate), overdrive_(1.0) {}
    
    virtual ~Overdrive();
    
    void controlChange( int number, StkFloat value ) {
        delegate_->controlChange(number, value);
    }

    //! Start a note with the given frequency and amplitude.
    void noteOn( StkFloat frequency, StkFloat amplitude ) {
        delegate_->noteOn(frequency, amplitude*overdrive_);
    }
    
    //! Stop a note with the given amplitude (speed of decay).
    void noteOff( StkFloat amplitude ) {
        delegate_->noteOff(amplitude);
    }
    
    //! Set instrument parameters for a particular frequency.
    void setFrequency( StkFloat frequency ) {
        delegate_->setFrequency(frequency);
    }
    
    void setOverdrive( StkFloat overdrive ) {
        overdrive_ = overdrive;
    }
    
    //! Compute one sample frame and return the specified \c channel value.
    /*!
     For monophonic instruments, the \c channel argument is ignored.
     */
    StkFloat tick( unsigned int channel = 0 );
    
    //! Fill the StkFrames object with computed sample frames, starting at the specified channel.
    /*!
     The \c channel argument plus the number of output channels must
     be less than the number of channels in the StkFrames argument (the
     first channel is specified by 0).  However, range checking is only
     performed if _STK_DEBUG_ is defined during compilation, in which
     case an out-of-range value will trigger an StkError exception.
     */
    StkFrames& tick( StkFrames& frames, unsigned int channel = 0 );

 protected:
    
    Instrmnt* delegate_;
    StkFloat overdrive_;

};

inline StkFloat Overdrive :: tick( unsigned int )
{
  StkFloat value = delegate_->tick();
    //value *= overdrive_;
    lastFrame_[0] = value / overdrive_;
  return lastFrame_[0];
}

inline StkFrames& Overdrive :: tick( StkFrames& frames, unsigned int channel )
{
  unsigned int nChannels = lastFrame_.channels();
#if defined(_STK_DEBUG_)
  if ( channel > frames.channels() - nChannels ) {
    oStream_ << "HevyMetl::tick(): channel and StkFrames arguments are incompatible!";
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
