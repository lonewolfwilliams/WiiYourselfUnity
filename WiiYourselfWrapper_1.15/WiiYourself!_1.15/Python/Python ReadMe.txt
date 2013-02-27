
 WiiYourself! Python Bindings - by Robert Xiao (nneonneo@gmail.com) August 2009
__________________________________________________________________________________

 please direct all questions about this wrapper to the WiiYourself! 
  mailing list:  http://gl.tter.org/mailman/listinfo/wiiyourself_gl.tter.org 
__________________________________________________________________________________


This Python wrapper is designed for SWIG.  It was tested with Python v2.6,
but may also work with other versions.

A precompiled version of the library for Python 2.6 is included.

Build Instructions:

Acquire Python and a suitable compiler, and just run "setup.py install". To build from scratch, you will need SWIG; run "swig -python -c++ -threads wiimote.i" to generate the wrapper and "setup.py install" to build and install it. No other dependencies are required.

Notes:

I decided not to use Boost::Python, as it was a big hassle trying to make it compile, and it still crashes when running a simple program, so it's back to SWIG. I toyed around with the idea of using a hack to maintain the nested structs structure (in wiimote_state.h) to make SWIG happy, but alas, SWIG just won't take it. (however, DESWIGIFY does maintain the structure insofar as it typedef's the globally promoted names back to the right spots, so, for example, you can still refer to wiimote_state_event as state_event within the wiimote class, because of the typedef).

Robert

