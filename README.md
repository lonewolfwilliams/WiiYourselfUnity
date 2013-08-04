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

-----------------
project structure
-----------------

Wiiyourself is compiled as dll, which in turn is consumed by Unity through WiiMoteBridge (interop code)

-------------------
directory structure
-------------------

/Debug: 
/Release: project builds for all the dependencies of WiiYourselfBridge for Unity (not really of interest)

/UnityExampleProject: Sample project showing how to use the library in Unity

/WiiYourselfWrapper_1.15:
	/WiiYourself_1.15: Gl.itters original (unmodified) Wiiyourself library, visual Studio 2008 project builds a static library
	/WiiYourselfWrapper: The wrapper for the WiiYourself static Library, visual studio 2010 project builds a .dll wrapping the static library

/WiiYourselfUnity: The WiiYourselfBridge, visual studio 2010 project builds a test harness console application that consumes the wrapper dll via interop
	/WiiYourselfBridge.cs: the c# class you need to use in your unity project to consume the dll via interop

ALL THE SOLUTION FILES FOR BUILDING THE LIBRARY ARE IN THE ROOT OF THE REPO

--------
Overview
--------

The original Wiiyourself library by gl.itter is a static library, in order to consume this in unity it needs to be wrapped in a c# dll, which is then loaded via 
the c# class WiiYourselfBridge, this class provides the interface onto the dll. Please refer to the sample project if you need more clarification.

If you just want to use the library 'as is' and not modify it you can just use the sample unity project as your guide.

If you want to extend the functionality of the library, read on...

---------------------
Modifying the library
---------------------

The projects are created in such a way that you should be able to build them with a minimum of fuss. You will receive a folder at the end with the dll and the bridge class all 
in the correct relative placement; You can copy the contents of the build directory into your unity project and use the existing sample project as your guide. The project also creates 
an exe file in the root of the build directory that you can use to test the modifications you made.

To add functionality to the library you need to add methods to the wrapper dll that calls the functionality on the static library, you then also need to add the corresponding 
interop call to the bridge class to make the wrapped call available to unity.

The function names in the wrapper dll will be mangled, you need to use the dumpbin.exe utility (a command line tool that comes with the visual studio c++ install) to decipher
what the mangled names (entrypoints) are. You need to run this from the VS 20XX command prompt as an administrator.

so, taking an existing function from the static library (WiiYourself_1.15/wiimote.h):
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

I ran dumpbin to find the mangled entrypoint (I'm sure there are better ways than doing this):
(run cmd for vs2010 as admin)
(navigate to correct directory)
dumpbin WiiYourselfWrapper.dll /EXPORTS 
1    0 00006AC0 ?Connect@@YA_NI_N@Z = ?Connect@@YA_NI_N@Z (bool __cdecl Connect(unsigned int,bool))

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

ALL THE SOLUTION FILES FOR BUILDING THE LIBRARY ARE IN THE ROOT OF THE REPO

Now you have two options:

------------------------------------------------------
building under Visual Studio 2010 Professional edition
------------------------------------------------------

If you don't have VS2010 pro, you could consider getting a 30 day evaluation version...

The solution file you need to open is: WiiYourselfUnitySolution.sln

Once you have this installed the Visual Studio 2010 project is configured so that you should 'just' be able to build the solution and get a console executable that can be used as a test harness for the wiimote (of course you need the wiimote paired to your computer first.)
There is a guide to pairing in UnityExampleProject/readme.html I have found that I need remove the wiimote device and add it again every time I start windows, others seem to find that the Blue Soleil bluetooth stack mitigates this requirement I have not tested that.

The test harness executable appears in the default directory $(SolutionDir)/bin/$(configuration) 
EG: /WiiYourselfUnity/bin/Debug/WiiYourselfUnity.exe

The wiimoteBridge.cs file is copied to: $(SolutionDir)$(ProjectName)\bin\$(Configuration) - this provides the api onto the dll for your unity code (via interop)
EG: /WiiYourselfUnity/bin/Debug/WiimoteBridge.cs

The WiiMote wrapper is copied to $(SolutionDir)$(SolutionName)/bin/$(Configuration)/Assets/Lib which is the same location that it needs to reside in the Unity project. 
EG: /WiiYourselfUnity/bin/Debug/Assets/Lib/WiiYourselfWrapper.dll

All other build products go to their default directory $(SolutionDir)$(configuration) these are probably not of interest to you...
EG: /Debug/WiiYourselfUnity.exe
EG: /Debug/WiiYourselfWrapper.dll

you should now be able to copy the contents of /WiiYourselfUnity/bin/(Debug or Release) into your unity project. (see sample project as a reference)

--------------------------------------------------
Building under Visual Studio 2010 Express editions
--------------------------------------------------

There doesn't seem to be a visual studio express edition that supports multiple project types (cpp / c#) in one solution, so you need to download *both*
- Visual Studio Express 2010 for visual cpp (opens WiiYourselfUnitySolution_VS2010_CPP)
- Visual Studio Express 2010 for visual c# (opens WiiYourselfUnitySolution_VS2010_CSharp)

you will find that windows wants to associate the .sln files with a single IDE so it is best to open visual studio first, then use file->open to open the relevent .sln

you need to build the project in the following order:

1 - build the CPP project in VS2010 for visual cpp
2 - build the c# project in VS2010 for visual c# (this will have dependencies on files produced in step 1)

now you should have a console executable that can be used as a test harness for the wiimote (of course you need the wiimote paired to your computer first.)
There is a guide to pairing in UnityExampleProject/readme.html I have found that I need remove the wiimote device and add it again every time I start windows, others seem to find that the Blue Soleil bluetooth stack mitigates this requirement I have not tested that.

The test harness executable appears in the default directory $(SolutionDir)/bin/$(configuration) 
EG: /WiiYourselfUnity/bin/Debug/WiiYourselfUnity.exe

The wiimoteBridge.cs file is copied to: $(SolutionDir)$(ProjectName)\bin\$(Configuration) - this provides the api onto the dll for your unity code (via interop)
EG: /WiiYourselfUnity/bin/Debug/WiimoteBridge.cs

The WiiMote wrapper is copied to $(SolutionDir)$(SolutionName)/bin/$(Configuration)/Assets/Lib which is the same location that it needs to reside in the Unity project. 
EG: /WiiYourselfUnity/bin/Debug/Assets/Lib/WiiYourselfWrapper.dll

All other build products go to their default directory $(SolutionDir)$(configuration) these are probably not of interest to you...
EG: /Debug/WiiYourselfUnity.exe
EG: /Debug/WiiYourselfWrapper.dll

you should now be able to copy the contents of /WiiYourselfUnity/bin/(Debug or Release) into your unity project. (see sample project as a reference)

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