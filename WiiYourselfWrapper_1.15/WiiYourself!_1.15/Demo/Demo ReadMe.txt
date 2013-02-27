________________________________________________________________

    - WiiYourself! - native C++ Wiimote library  v1.15
      (c) gl.tter 2007-10 - http://gl.tter.org
________________________________________________________________

This demo app tries to connect to the first Wiimote in the
system, and shows the most important incoming data.

It mainly uses polling (the library is constantly updating
the wiimote state data behind the scenes) but also shows the
callback mechanism by reacting to extension connection changes.
The choice between polling, callbacks, or a mixture of the two
is entirely up to the application.

 - a pre-compiled binary is included.
 - as with the library, the WinDDK is required to build:
    http://www.microsoft.com/whdc/devtools/ddk/default.mspx
  
Known issues: Extensions connected to a MotionPlus are usually
not detected correctly.  The problem is the
wiimote::DisableMotionPlus() which doesn't work reliably.
Fixes are welcome.

Sign up to the mailing list to stay in the loop, give feedback,
exchange ideas or get involved:
http://gl.tter.org/mailman/listinfo/wiiyourself_gl.tter.org
__

gl.tter  (glATr-i-lDOTnet)

