/***************************************************/
/*! \class SampleInstrument
 \brief Wrapper for FileWave
 
 Based on VoicForm
 by Perry R. Cook and Gary P. Scavone, 1995-2012.
 */
/***************************************************/


#include "SampleInstrument.h"
#include "Phonemes.h"
#include "SKINI.msg"
#include <cstring>
#include <cmath>

namespace stk {

SampleInstrument :: SampleInstrument( std::string fileName ) : Instrmnt()
{
  // Concatenate the STK rawwave path to the rawwave file
  voiced_ = new FileWave(fileName.c_str(), false );
  voiced_->setGainRate( 0.001 );
  voiced_->setGainTarget( 0.0 );;
}  

SampleInstrument :: ~SampleInstrument( void )
{
  delete voiced_;
}

void SampleInstrument :: setFrequency( StkFloat frequency )
{
#if defined(_STK_DEBUG_)
  if ( frequency <= 0.0 ) {
    oStream_ << "VoicForm::setFrequency: parameter is less than or equal to zero!";
    handleError( StkError::WARNING ); return;
  }
#endif

  voiced_->setFrequency( frequency );
}


void SampleInstrument :: noteOn( StkFloat frequency, StkFloat amplitude )
{
  this->setFrequency( frequency );
  voiced_->setGainTarget( amplitude );
}

void SampleInstrument :: controlChange( int number, StkFloat value )
{
#if defined(_STK_DEBUG_)
  if ( Stk::inRange( value, 0.0, 128.0 ) == false ) {
    oStream_ << "Clarinet::controlChange: value (" << value << ") is out of range!";
    handleError( StkError::WARNING ); return;
  }
#endif

  StkFloat normalizedValue = value * ONE_OVER_128;
  if (number == __SK_ModFrequency_) // 11
    voiced_->setVibratoRate( normalizedValue * 12.0);  // 0 to 12 Hz
  else if (number == __SK_ModWheel_) // 1
    voiced_->setVibratoGain( normalizedValue * 0.2);

#if defined(_STK_DEBUG_)
  else {
    oStream_ << "VoicForm::controlChange: undefined control number (" << number << ")!";
    handleError( StkError::WARNING );
  }
#endif
}

} // stk namespace
