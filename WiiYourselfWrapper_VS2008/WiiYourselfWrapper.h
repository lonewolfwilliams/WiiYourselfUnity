/*
* The BSD 3-Clause License The following is a BSD 3-Clause ("BSD New" or "BSD Simplified") license template. To generate your own license, change the values of OWNER, ORGANIZATION and YEAR from their original values as given here, and substitute your own.
* Note: You may omit clause 3 and still be OSD-conformant. Despite its colloquial name "BSD New", this is not the newest version of the BSD license; it was followed by the even newer BSD-2-Clause version, sometimes known as the "Simplified BSD License". On January 9th, 2008 the OSI Board approved BSD-2-Clause, which is used by FreeBSD and others. It omits the final "no-endorsement" clause and is thus roughly equivalent to the MIT License.
* Historical Background: The original license used on BSD Unix had four clauses. The advertising clause (the third of four clauses) required you to acknowledge use of U.C. Berkeley code in your advertising of any product using that code. It was officially rescinded by the Director of the Office of Technology Licensing of the University of California on July 22nd, 1999. He states that clause 3 is "hereby deleted in its entirety." The four clause license has not been approved by OSI. The license below does not contain the advertising clause.
* This prelude is not part of the license.
* = Regents of the University of California = University of California, Berkeley = 1998
* In the original BSD license, both occurrences of the phrase "COPYRIGHT HOLDERS AND CONTRIBUTORS" in the disclaimer read "REGENTS AND CONTRIBUTORS".
* Here is the license template:
* Copyright (c) , All rights reserved.
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. Neither the name of the nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
* Gareth Williams: https://github.com/lonewolfwilliams
* Wraps WiiYourself Library written by gl.tter: http://wiiyourself.gl.tter.org/

* used
* http://waterhobo.com/?p=28
* http://rudematrix.co.uk/?p=322
* as references
*/

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WII_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WII_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

//http://www.codeproject.com/Articles/28969/HowTo-Export-C-classes-from-a-DLL

#ifdef WII_EXPORTS
#define WII_API __declspec(dllexport)
#else
#define WII_API __declspec(dllimport)
#endif

#pragma once

#include "wiimote.h"
#include "wiimote_state.h"

#define CONNECTED 1

wiimote* m_wiimote = 0;

// call Connect() first - returns true if wiimote was found & enabled
//  - 'wiimote_index' specifies which *installed* (not necessarily
//     *connected*) wiimote should be tried (1 = first, 2 = 2nd etc).
//    if you just want the first *available* wiimote that isn't already
//     in use, pass in FIRST_AVAILABLE (default).
//  - 'force_hidwrites' forces HID output method (it's auto-selected
//     when needed and less efficient, so only force for testing).
extern WII_API const unsigned FIRST_AVAILABLE = 0xffffffff;
WII_API bool Connect(unsigned wiimote_index = FIRST_AVAILABLE,
					bool force_hidwrites = false);

// disconnect from the controller and stop reading data from it
WII_API void Disconnect (void);

// set/clear the wiimote LEDs
WII_API void SetLEDs (unsigned char led_bits); // bits 0-3 are valid

//accelerometer accessors
WII_API float GetXAcceleration();
WII_API float GetYAcceleration();
WII_API float GetZAcceleration();

//gyro accessors
WII_API float GetXOrientation();
WII_API float GetYOrientation();
WII_API float GetZOrientation();