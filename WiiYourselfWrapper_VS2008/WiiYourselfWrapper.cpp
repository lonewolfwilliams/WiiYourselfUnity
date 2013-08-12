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

#include "stdafx.h"
#include "WiiYourselfWrapper.h"

/*
* Gareth Williams: https://github.com/lonewolfwilliams
* Wraps WiiYourself Library written by gl.tter: http://wiiyourself.gl.tter.org/
*/

//on connection we need to set the correct report mode...
//todo: test and return for more modes including nunchuk etc...
void on_state_change (wiimote			  &remote,
					  state_change_flags  changed,
					  const wiimote_state &new_state)
{
	// the wiimote just connected
	if(changed & CONNECTED)
	{
		// ask the wiimote to report everything (using the 'non-continous updates'
		//  default mode - updates will be frequent anyway due to the acceleration/IR
		//  values changing):

		// note1: you don't need to set a report type for Balance Boards - the
		//		   library does it automatically.
		
		// note2: for wiimotes, the report mode that includes the extension data
		//		   unfortunately only reports the 'BASIC' IR info (ie. no dot sizes),
		//		   so let's choose the best mode based on the extension status:
			/*
		if(new_state.ExtensionType != wiimote::BALANCE_BOARD)
		{
		if(new_state.bExtension)
			remote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR_EXT); // no IR dots
		else
			remote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR);		//    IR dots
		}*/
	
		//assume regular wiimote with nothing connected to it...
		remote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR);
	}
}

WII_API bool Connect(unsigned wiimote_index,
					bool force_hidwrites)
{
	if(m_wiimote == 0)
	{
		m_wiimote = new wiimote();
		m_wiimote->ChangedCallback = on_state_change;
		m_wiimote->CallbackTriggerFlags = (state_change_flags)(CONNECTED);
	}
	return m_wiimote->Connect(wiimote_index, force_hidwrites);
}

WII_API void Disconnect (void)
{
	m_wiimote->Disconnect();
	delete m_wiimote;
	m_wiimote = 0;
}

WII_API int Update()
{
	return (int)m_wiimote->RefreshState();
}

//going out

WII_API void SetLEDs (unsigned char led_bits)
{
	m_wiimote->SetLEDs(led_bits);
}

//coming in

//todo: we can make this more efficient later by grouping accelerometer data returned
WII_API float GetXOrientation()
{
	return m_wiimote->Acceleration.Orientation.X;
}
WII_API float GetYOrientation()
{
	return m_wiimote->Acceleration.Orientation.Y;
}
WII_API float GetZOrientation()
{
	return m_wiimote->Acceleration.Orientation.Z;
}

WII_API int GetPitchDegrees()
{
	return m_wiimote->Acceleration.Orientation.Pitch;
}
WII_API int GetRollDegrees()
{
	return m_wiimote->Acceleration.Orientation.Roll;
}

WII_API float GetXAcceleration()
{
	return m_wiimote->Acceleration.X;
}
WII_API float GetYAcceleration()
{
	return m_wiimote->Acceleration.Y;
}
WII_API float GetZAcceleration()
{
	return m_wiimote->Acceleration.Z;
}
