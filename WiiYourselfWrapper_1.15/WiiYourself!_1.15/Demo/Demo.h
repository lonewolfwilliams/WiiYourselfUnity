// _______________________________________________________________________________
//
//	 - WiiYourself! - native C++ Wiimote library  v1.15
//	  (c) gl.tter 2007-10 - http://gl.tter.org
//
//	  see License.txt for conditions of use.  see History.txt for change log.
// _______________________________________________________________________________
//
//  demo.h  (tab = 4 spaces)

// print-related defines (so we don't clutter the .cpp)
#define _SETCOL(flags) SetConsoleTextAttribute(console, flags)

#define WHITE			_SETCOL(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE)
#define BRIGHT_WHITE	_SETCOL(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY)
#define RED				_SETCOL(FOREGROUND_RED)
#define BRIGHT_RED		_SETCOL(FOREGROUND_RED|FOREGROUND_INTENSITY)
#define BLUE			_SETCOL(FOREGROUND_BLUE)
#define BRIGHT_BLUE		_SETCOL(FOREGROUND_BLUE|FOREGROUND_INTENSITY)
#define GREEN			_SETCOL(FOREGROUND_GREEN)
#define BRIGHT_GREEN	_SETCOL(FOREGROUND_GREEN|FOREGROUND_INTENSITY)
#define YELLOW			_SETCOL(FOREGROUND_RED|FOREGROUND_GREEN)
#define BRIGHT_YELLOW	_SETCOL(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY)
#define CYAN			_SETCOL(FOREGROUND_BLUE|FOREGROUND_GREEN)
#define BRIGHT_CYAN		_SETCOL(FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY)
#define PURPLE			_SETCOL(FOREGROUND_RED|FOREGROUND_BLUE)
#define BRIGHT_PURPLE	_SETCOL(FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_INTENSITY)

#define BLANK_LINE \
 _T("                                                                             \n")
