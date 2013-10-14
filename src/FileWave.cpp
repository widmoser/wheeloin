/***************************************************/
/*! \class FileWave
 \brief General class for using looped sound files.
 
 This class is based on SingWave
 by Perry R. Cook and Gary P. Scavone, 1995-2012.
 */
/***************************************************/

#include "FileWave.h"
#include <iostream>

namespace stk {
 
FileWave :: FileWave( std::string fileName, bool raw )
{
  // An exception could be thrown here.
  wave_.openFile( fileName, raw );

	rate_ = 1.0;
	sweepRate_ = 0.001;

	modulator_.setVibratoRate( 6.0 );
	modulator_.setVibratoGain( 0.04 );
	modulator_.setRandomGain( 0.005 );

	this->setFrequency( 77.7817 );
	pitchEnvelope_.setRate( 1.0 );
	this->tick();
	this->tick();
	pitchEnvelope_.setRate( sweepRate_ * rate_ );
}

FileWave :: ~FileWave()
{
}

void FileWave :: setFrequency( StkFloat frequency )
{
	StkFloat temp = rate_;
	rate_ = frequency / 77.7817;
	temp -= rate_;
	if ( temp < 0) temp = -temp;
	pitchEnvelope_.setTarget( rate_ );
	pitchEnvelope_.setRate( sweepRate_ );
}

} // stk namespace
