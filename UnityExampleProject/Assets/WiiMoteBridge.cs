using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using UnityEngine;

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
 * WiiYourself Unity:  depends on the WiiYourself library originally authored by gl.tter: http://wiiyourself.gl.tter.org/
 * 
 */

/*
 * Gareth Williams
 * 
 * abstracts the pinvoke calls away from the unity monobehaviour
 * 
 */

namespace WiiYourselfWrapperHarness
{
    class WiiMoteBridge
    {
        //using mangled names from dumpbin util - poor form.
        [DllImport("Assets/lib/WiiYourselfWrapper.dll", EntryPoint = "?Connect@@YA_NI_N@Z", CallingConvention = CallingConvention.Cdecl)]
        static extern bool WiiConnect(uint wiimote_index, bool force_hidwrites);
        [DllImport("Assets/lib/WiiYourselfWrapper.dll", EntryPoint = "?Disconnect@@YAXXZ", CallingConvention = CallingConvention.Cdecl)]
        static extern void WiiDisconnect();
        [DllImport("Assets/lib/WiiYourselfWrapper.dll", EntryPoint = "?SetLEDs@@YAXE@Z", CallingConvention = CallingConvention.Cdecl)]
        static extern void WiiSetLEDs(byte led_bits);

        [DllImport("Assets/lib/WiiYourselfWrapper.dll", EntryPoint = "?Update@@YAHXZ", CallingConvention = CallingConvention.Cdecl)]
        static extern int WiiUpdate();

        [DllImport("Assets/lib/WiiYourselfWrapper.dll", EntryPoint = "?GetXOrientation@@YAMXZ", CallingConvention = CallingConvention.Cdecl)]
        static extern float GetXOrientation();
        [DllImport("Assets/lib/WiiYourselfWrapper.dll", EntryPoint = "?GetYOrientation@@YAMXZ", CallingConvention = CallingConvention.Cdecl)]
        static extern float GetYOrientation();
        [DllImport("Assets/lib/WiiYourselfWrapper.dll", EntryPoint = "?GetZOrientation@@YAMXZ", CallingConvention = CallingConvention.Cdecl)]
        static extern float GetZOrientation();

        [DllImport("Assets/lib/WiiYourselfWrapper.dll", EntryPoint = "?GetPitchDegrees@@YAHXZ", CallingConvention = CallingConvention.Cdecl)]
        static extern int GetPitchDegrees();
        [DllImport("Assets/lib/WiiYourselfWrapper.dll", EntryPoint = "?GetRollDegrees@@YAHXZ", CallingConvention = CallingConvention.Cdecl)]
        static extern int GetRollDegrees();

        [DllImport("Assets/lib/WiiYourselfWrapper.dll", EntryPoint = "?GetXAcceleration@@YAMXZ", CallingConvention = CallingConvention.Cdecl)]
        static extern float GetXAcceleration();
        [DllImport("Assets/lib/WiiYourselfWrapper.dll", EntryPoint = "?GetYAcceleration@@YAMXZ", CallingConvention = CallingConvention.Cdecl)]
        static extern float GetYAcceleration();
        [DllImport("Assets/lib/WiiYourselfWrapper.dll", EntryPoint = "?GetZAcceleration@@YAMXZ", CallingConvention = CallingConvention.Cdecl)]
        static extern float GetZAcceleration();
        
        byte m_ledFlags = 0;
        public enum PlayerLED
        {
            playerOne = 1,
            playerTwo = 2,
            playerThree = 4,
            playerFour = 8
        }

        public struct ThreeAxis //trying to disambiguate from vector3d or vector3
        {
            public float x,y,z;
        }
        ThreeAxis m_threeAxis = new ThreeAxis();

        public struct TwoAxis
        {
            public int pitch, roll;
        }
        TwoAxis m_twoAxis = new TwoAxis();

        public void TryConnect()
        {
            int maxAttempts = 100;
            int connectionAttempts = 0;
            bool success = false;
            do
            {
                success = WiiConnect(0xFFFFFFFF, false);
            }
            while (success == false && connectionAttempts++ < maxAttempts);

            System.Diagnostics.Debug.Assert(success, "wiimote connection failed - have you paired with bluetooth? is the battery low?");
        }

        public bool Update()
        {
            bool stateChanged = false;
            int flags = WiiUpdate();
            if (flags > 0) //more than no flags
            {
                stateChanged = true;
            }

            return stateChanged;
        }

        public void ToggleLED(PlayerLED forPlayer)
        {
            m_ledFlags ^= (byte)forPlayer;
            WiiSetLEDs(m_ledFlags);
        }

        public ThreeAxis GetOrientation()
        {
            m_threeAxis.x = GetXOrientation();
            m_threeAxis.y = GetYOrientation();
            m_threeAxis.z = GetZOrientation();

            return m_threeAxis;
        }

        public ThreeAxis GetAcceleration()
        {
            m_threeAxis.x = GetXOrientation();
            m_threeAxis.y = GetYOrientation();
            m_threeAxis.z = GetZOrientation();

            return m_threeAxis;
        }

        public TwoAxis GetPitchRoll()
        {
            m_twoAxis.pitch = GetPitchDegrees();
            m_twoAxis.roll  = GetRollDegrees();

            return m_twoAxis;
        }

        public void Disconnect()
        {
            WiiDisconnect();
        }
    }
}
