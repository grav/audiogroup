/*
*	File:		TremoloUnit.cpp
*	
*	Version:	1.0
* 
*	Created:	24/05/09
*	
*	Copyright:  Copyright © 2009 Betafunk, All Rights Reserved
* 
*	Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc. ("Apple") in 
*				consideration of your agreement to the following terms, and your use, installation, modification 
*				or redistribution of this Apple software constitutes acceptance of these terms.  If you do 
*				not agree with these terms, please do not use, install, modify or redistribute this Apple 
*				software.
*
*				In consideration of your agreement to abide by the following terms, and subject to these terms, 
*				Apple grants you a personal, non-exclusive license, under Apple's copyrights in this 
*				original Apple software (the "Apple Software"), to use, reproduce, modify and redistribute the 
*				Apple Software, with or without modifications, in source and/or binary forms; provided that if you 
*				redistribute the Apple Software in its entirety and without modifications, you must retain this 
*				notice and the following text and disclaimers in all such redistributions of the Apple Software. 
*				Neither the name, trademarks, service marks or logos of Apple Computer, Inc. may be used to 
*				endorse or promote products derived from the Apple Software without specific prior written 
*				permission from Apple.  Except as expressly stated in this notice, no other rights or 
*				licenses, express or implied, are granted by Apple herein, including but not limited to any 
*				patent rights that may be infringed by your derivative works or by other works in which the 
*				Apple Software may be incorporated.
*
*				The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO WARRANTIES, EXPRESS OR 
*				IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY 
*				AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE 
*				OR IN COMBINATION WITH YOUR PRODUCTS.
*
*				IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR CONSEQUENTIAL 
*				DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
*				OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, 
*				REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER 
*				UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN 
*				IF APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/
/*=============================================================================
	TremoloUnit.cpp
	
=============================================================================*/
#include "TremoloUnit.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(TremoloUnit)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::TremoloUnit
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TremoloUnit::TremoloUnit(AudioUnit component)
	: AUEffectBase(component)
{
   // This method, defined in the AUBase superclass, ensures that the required audio unit
   //  elements are created and initialized.
   CreateElements ();
   
   // Invokes the use of an STL vector for parameter access.  
   //  See AUBase/AUScopeElement.cpp
   Globals () -> UseIndexedParameters (kNumberOfParameters);
   
   // During instantiation, sets up the parameters according to their defaults.
   //  The parameter defaults should correspond to the settings for the default preset.
   SetParameter (
                 kParameter_Frequency, 
                 kDefaultValue_Tremolo_Freq 
                 );
   
   SetParameter (
                 kParameter_Depth, 
                 kDefaultValue_Tremolo_Depth 
                 );
   
   SetParameter (
                 kParameter_Waveform, 
                 kDefaultValue_Tremolo_Waveform 
                 );
   
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			TremoloUnit::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
   if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParameter_Waveform)) {
      // This method applies only to the waveform parameter, which is in the global scope.
      
      // When this method gets called by the AUBase::DispatchGetPropertyInfo method, which 
      // provides a null value for the outStrings parameter, just return without error.
      if (outStrings == NULL) return noErr;
      
      // Defines an array that contains the pop-up menu item names.
      CFStringRef  strings [] = {
         kMenuItem_Tremolo_Sine,
         kMenuItem_Tremolo_Square
      };
      
      // Creates a new immutable array containing the menu item names, and places the array 
      // in the outStrings output parameter.
      *outStrings = CFArrayCreate (
                                   NULL,
                                   (const void **) strings,
                                   (sizeof (strings) / sizeof (strings [0])),
                                   NULL
                                   );
      return noErr;
   }
return kAudioUnitErr_InvalidParameter;}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			TremoloUnit::GetParameterInfo(AudioUnitScope		inScope,
                                                        AudioUnitParameterID	inParameterID,
                                                        AudioUnitParameterInfo	&outParameterInfo )
{

   ComponentResult result = noErr;
   
   // Adds two flags to all parameters for the audio unit, indicating to the host application 
   // that it should consider all the audio unit's parameters to be readable and writable.
   outParameterInfo.flags =     
   kAudioUnitParameterFlag_IsWritable | kAudioUnitParameterFlag_IsReadable;
   
   // All three parameters for this audio unit are in the "global" scope.
   if (inScope == kAudioUnitScope_Global) {
      switch (inParameterID) {
            
         case kParameter_Frequency:
            // Invoked when the view needs information for the kTremoloParam_Frequency 
            // parameter; defines how to represent this parameter in the user interface.
            AUBase::FillInParameterName (
                                         outParameterInfo,
                                         kParamName_Tremolo_Freq,
                                         false
                                         );
            outParameterInfo.unit      = kAudioUnitParameterUnit_Hertz;
            // Sets the unit of measurement for the Frequency parameter to Hertz.
            outParameterInfo.minValue    = kMinimumValue_Tremolo_Freq;
            // Sets the minimum value for the Frequency parameter.
            outParameterInfo.maxValue    = kMaximumValue_Tremolo_Freq;
            // Sets the maximum value for the Frequency parameter.
            outParameterInfo.defaultValue  = kDefaultValue_Tremolo_Freq;
            // Sets the default value for the Frequency parameter.
            outParameterInfo.flags      |= kAudioUnitParameterFlag_DisplayLogarithmic;
            // Adds a flag to indicate to the host that it should use a logarithmic 
            // control for the Frequency parameter.
            break;
            
         case kParameter_Depth:
            // Invoked when the view needs information for the kTremoloParam_Depth parameter.
            AUBase::FillInParameterName (
                                         outParameterInfo,
                                         kParamName_Tremolo_Depth,
                                         false
                                         );
            outParameterInfo.unit      = kAudioUnitParameterUnit_Percent;
            outParameterInfo.minValue    = kMinimumValue_Tremolo_Depth;
            outParameterInfo.maxValue    = kMaximumValue_Tremolo_Depth;
            outParameterInfo.defaultValue  = kDefaultValue_Tremolo_Depth;
            break;
            
         case kParameter_Waveform:
            // Invoked when the view needs information for the kTremoloParam_Waveform parameter.
            AUBase::FillInParameterName (
                                         outParameterInfo,
                                         kParamName_Tremolo_Waveform,
                                         false
                                         );
            outParameterInfo.unit      = kAudioUnitParameterUnit_Indexed;
            // Sets the unit of measurement for the Waveform parameter to "indexed," allowing 
            // it to be displayed as a pop-up menu in the generic view. The following three 
            // statements set the minimum, maximum, and default values for the depth parameter. 
            // All three are required for proper functioning of the parameter's user interface.
            outParameterInfo.minValue    = kSineWave_Tremolo_Waveform;
            outParameterInfo.maxValue    = kSquareWave_Tremolo_Waveform;
            outParameterInfo.defaultValue  = kDefaultValue_Tremolo_Waveform;
            break;
            
         default:
            result = kAudioUnitErr_InvalidParameter;
            break;
      }
   } else {
      result = kAudioUnitErr_InvalidParameter;
   }
   return result;

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			TremoloUnit::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			TremoloUnit::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}


#pragma mark ____TremoloUnitEffectKernel


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::TremoloUnitKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		TremoloUnit::TremoloUnitKernel::Reset()
{
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::TremoloUnitKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		TremoloUnit::TremoloUnitKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, // for version 2 AudioUnits inNumChannels is always 1
                                                    bool			&ioSilence )
{

	//This code will pass-thru the audio data.
	//This is where you want to process data to produce an effect.

	
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	Float32 gain = 0;//GetParameter( kParam_One );
	
	while (nSampleFrames-- > 0) {
		Float32 inputSample = *sourceP;
		
		//The current (version 2) AudioUnit specification *requires* 
	    //non-interleaved format for all inputs and outputs. Therefore inNumChannels is always 1
		
		sourceP += inNumChannels;	// advance to next frame (e.g. if stereo, we're advancing 2 samples);
									// we're only processing one of an arbitrary number of interleaved channels

			// here's where you do your DSP work
                Float32 outputSample = inputSample * gain;
		
		*destP = outputSample;
		destP += inNumChannels;
	}
}

