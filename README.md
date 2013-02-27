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
/Release: project builds for all the dependencies of WiiYourselfBridge for Unity

/UnityExampleProject: Sample project showing how to use the library in Unity

/WiiYourselfWrapper_1.15: The Dll wrapper for the WiiYourself static Library and it's dependency projects (Wiiyourself_1.15)

/WiiYourselfUnity: The WiiYourselfBridge, hosted in a testharness console application (Build this for the unity project)

--------------------
Building the library
--------------------

The project has a dependency on WinDDK which is configured for the default install directory (c:/winDDK) the library is currently tested with version 7600.16385.1 you can download it here: http://www.microsoft.com/en-us/download/details.aspx?id=11800
You may need to ammend this in the WiiYourself and WiiYourselfWrapper projects if you have a different build

Once you have this installed the Visual Studio 2010 project is configured so that you should 'just' be able to build the solution and get a console executable that can be used as a test harness for the wiimote (of course you need the wiimote paired to your computer first.)
There is a guide to pairing in UnityExampleProject/readme.html I have found that I need remove the wiimote device and add it again every time I start windows, others seem to find that the Blue Soleil bluetooth stack mitigates this requirement I have not tested that.

The executable appears in the default directory $(SolutionDir)/bin/$(configuration) 

The WiiMote wrapper is copied to $(SolutionDir)$(SolutionName)/bin/$(Configuration)/Assets/Lib which is the same location that it needs to reside in the Unity project. The reason for this is that you can then copy your WiiMoteBridge class into the Assets folder, and bind to that using a MonoBehaviour (see the bundled sample project) with a minimum of fuss.

All other projects build to the $(SolutionDir)$(configuration) directory

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