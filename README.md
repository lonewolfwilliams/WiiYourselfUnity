---------------------------
WiiYourself in Unity source
---------------------------

This is a managed wrapper by LoneWolfWilliams: https://github.com/lonewolfwilliams
for the WiiYourself library authored by gl.tter: http://wiiyourself.gl.tter.org/ (used with permission)

This project also includes a sample Unity scene showing how you can use the wrapper in a Unity Project for Unity Free Edition.

The good:
This library works with Unity free edition
This library doesn't cost anything

The bad:
!!! This library is for Windows only !!!
!!! No support - it's free after all ;) !!!

The ugly:
Bluetooth connectivity with the wiimote is as reliable as your bluetooth stack is compatible with wiimotes

This code is distributed under a BSD3 licence

-----------------
project structure
-----------------

Wiiyourself is compiled as dll, which in turn is consumed by Unity through WiiMoteBridge (interop code)

-------------------
directory structure
-------------------

/UnityExampleProject: Sample project showing how to use the library in Unity  
/WiiYourselfWrapper_VS2008: Visual Studio 2008 C++ Project - An unmanaged C++ DLL that wraps the final version of gl.itters wiiyourself library (version 1.15)
/WiiYourselfWrapper_VS2010: Visual Studio 2010 C++ Project - An unmanaged C++ DLL that wraps the final version of gl.itters wiiyourself library (version 1.15)
/HowToConnectWiimote: HTML file describing how to connect a Wiimote on Windows 
/WiiMoteBridge.cs: Interop code to bridge the unmanaged DLL so it can be used in Unity (see sample for details)

--------
Overview
--------

in order to consume this the original Wiiyourself driver by gl.itter in unity it needs to be wrapped in a c++ dll, which is then loaded via 
the c# class WiiYourselfBridge, this class provides the interface onto the dll. Please refer to the sample project if you need more clarification.

If you just want to use the library 'as is' and not modify it you can just use the sample unity project as your guide.

If you want to extend the functionality of the library, read on...

---------------------
Modifying the library
---------------------

To add functionality to the library you need to add methods to the wrapper dll that calls the functionality on gl.itters driver, you then also need to add the corresponding 
interop call to the bridge class to make the wrapped call available to unity.

The function names in the wrapper dll will be mangled (CDecl decorated), you need to use the dumpbin.exe utility (a command line tool that comes with the visual studio c++ install) to decipher
what the mangled names (entrypoints) are. You need to run this from the VS 20XX command prompt as an administrator.

Taking an existing function from the driver code as an example (WiiYourself_1.15/wiimote.h):
<code>
bool Connect (unsigned wiimote_index = FIRST_AVAILABLE, bool force_hidwrites = false);
</code>

I added the following to the wrapper dll (WiiYourselfWrapper/wiiYourselfWrapper.h):
<code>
WII_API bool Connect(unsigned wiimote_index = FIRST_AVAILABLE, bool force_hidwrites = false);
</code>
(WiiYourselfWrapper/wiiYourselfWrapper.cpp):
<code>
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
</code>

I ran dumpbin to find the mangled / decorated entrypoint (I'm sure there are better ways than doing this):
(run cmd for vs2010 as admin)
(navigate to correct directory)
dumpbin WiiYourselfWrapper.dll /EXPORTS 
<code>
1    0 00006AC0 ?Connect@@YA_NI_N@Z = ?Connect@@YA_NI_N@Z (bool __cdecl Connect(unsigned int,bool))
</code>

I then added the following to WiiMoteBridge.cs:
<code>
[DllImport("Assets/lib/WiiYourselfWrapper.dll", EntryPoint = "?Connect@@YA_NI_N@Z", CallingConvention = CallingConvention.Cdecl)]
static extern bool WiiConnect(uint wiimote_index, bool force_hidwrites);
</code>

--------------------
Building the library
--------------------

The project has a dependency on WinDDK which is configured for the default install directory (c:/winDDK) the library is currently tested with version 7600.16385.1 you can download it here: http://www.microsoft.com/en-us/download/details.aspx?id=11800
You may need to ammend this in the WiiYourself and WiiYourselfWrapper projects if you have a different build

To open the solution file you will need visual studio 2008 express for C++ or visual studio 2010 express for C++ (you may also be able to convert the project in more recent editions of the IDE, this is untested)

-----
To do
-----

Only a small amount of gl.tters extensive library is wrapped here, over time I would like to map more functionality, 
you are of course, welcome to help me by submitting your own patches ;)

----------
references
----------

The following blog entries were a big help - thanks guys!

http://waterhobo.com/?p=28
http://rudematrix.co.uk/?p=322