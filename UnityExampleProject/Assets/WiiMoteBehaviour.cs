using UnityEngine;
using System.Collections;
using WiiYourselfWrapperHarness;
using System.Collections.Generic;

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
 */

public class WiiMoteBehaviour : MonoBehaviour 
{
    WiiMoteBridge m_wiimote = new WiiMoteBridge();
   
    List<WiiMoteBridge.PlayerLED> m_buttons = new List<WiiMoteBridge.PlayerLED>()
    {
        WiiMoteBridge.PlayerLED.playerOne,
        WiiMoteBridge.PlayerLED.playerTwo,
        WiiMoteBridge.PlayerLED.playerThree,
        WiiMoteBridge.PlayerLED.playerFour
    };

	// Use this for initialization
	void Start () 
    {
        m_wiimote.TryConnect();
        UnityEngine.Debug.Log("connected");
	}

    void Update()
    {
        bool stateChange = m_wiimote.Update();
        //UnityEngine.Debug.Log("wiimote changed: " +  stateChange);
        WiiMoteBridge.ThreeAxis orientation = m_wiimote.GetOrientation();
        //Debug.Log(string.Format("orientation x:{0} y:{1} z:{2}", 
        //    orientation.x, orientation.y, orientation.z));
        WiiMoteBridge.ThreeAxis acceleration = m_wiimote.GetAcceleration();
        //Debug.Log(string.Format("acceleration x:{0} y:{1} z:{2}", 
        //    acceleration.x, acceleration.y, acceleration.z));
        WiiMoteBridge.TwoAxis pitchRoll = m_wiimote.GetPitchRoll();
        //Debug.Log(string.Format("pitch roll p:{0} r:{1}",
        //    pitchRoll.pitch, pitchRoll.roll));

        //this.transform.Rotate(orientation.x, orientation.y, orientation.z);
        this.transform.rotation = Quaternion.Euler(pitchRoll.pitch, 0, pitchRoll.roll);
    }
    
    void OnGUI()
    {
        if (GUI.Button(new Rect(10, 10, 150, 100), "change wiimote light"))
        {
            int randomIndex = Mathf.RoundToInt(Random.value * 3);
            m_wiimote.ToggleLED(m_buttons[randomIndex]);
        }
    }
    
    void OnDestroy()
    {
        m_wiimote.Disconnect();
    }
}
