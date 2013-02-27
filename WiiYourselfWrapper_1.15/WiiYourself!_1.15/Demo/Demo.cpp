// _______________________________________________________________________________
//
//	 - WiiYourself! - native C++ Wiimote library  v1.15
//	  (c) gl.tter 2007-10 - http://gl.tter.org
//
//	  see License.txt for conditions of use.  see History.txt for change log.
// _______________________________________________________________________________
//
//  demo.cpp  (tab = 4 spaces)
#include "Demo.h"
#include "..\wiimote.h"
#include <mmsystem.h>	// for timeGetTime

// configs:
#define USE_BEEPS_AND_DELAYS			// undefine to test library works without them
#define LOOK_FOR_ADDITIONAL_WIIMOTES	// tries to connect any extra wiimotes

// ------------------------------------------------------------------------------------
//  state-change callback example (we use polling for everything else):
// ------------------------------------------------------------------------------------
void on_state_change (wiimote			  &remote,
					  state_change_flags  changed,
					  const wiimote_state &new_state)
	{
	// we use this callback to set report types etc. to respond to key events
	//  (like the wiimote connecting or extensions (dis)connecting).
	
	// NOTE: don't access the public state from the 'remote' object here, as it will
	//		  be out-of-date (it's only updated via RefreshState() calls, and these
	//		  are reserved for the main application so it can be sure the values
	//		  stay consistent between calls).  Instead query 'new_state' only.

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
		if(new_state.ExtensionType != wiimote::BALANCE_BOARD)
			{
			if(new_state.bExtension)
				remote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR_EXT); // no IR dots
			else
				remote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR);		//    IR dots
			}
		}
	// a MotionPlus was detected
	if(changed & MOTIONPLUS_DETECTED)
		{
		// enable it if there isn't a normal extension plugged into it
		// (MotionPlus devices don't report like normal extensions until
		//  enabled - and then, other extensions attached to it will no longer be
		//  reported (so disable the M+ when you want to access them again).
		if(remote.ExtensionType == wiimote_state::NONE) {
			bool res = remote.EnableMotionPlus();
			_ASSERT(res);
			}
		}
	// an extension is connected to the MotionPlus
	else if(changed & MOTIONPLUS_EXTENSION_CONNECTED)
		{
		// We can't read it if the MotionPlus is currently enabled, so disable it:
		if(remote.MotionPlusEnabled())
			remote.DisableMotionPlus();
		}
	// an extension disconnected from the MotionPlus
	else if(changed & MOTIONPLUS_EXTENSION_DISCONNECTED)
		{
		// enable the MotionPlus data again:
		if(remote.MotionPlusConnected())
			remote.EnableMotionPlus();
		}
	// another extension was just connected:
	else if(changed & EXTENSION_CONNECTED)
		{
#ifdef USE_BEEPS_AND_DELAYS
		Beep(1000, 200);
#endif
		// switch to a report mode that includes the extension data (we will
		//  loose the IR dot sizes)
		// note: there is no need to set report types for a Balance Board.
		if(!remote.IsBalanceBoard())
			remote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR_EXT);
		}
	// extension was just disconnected:
	else if(changed & EXTENSION_DISCONNECTED)
		{
#ifdef USE_BEEPS_AND_DELAYS
		Beep(200, 300);
#endif
		// use a non-extension report mode (this gives us back the IR dot sizes)
		remote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR);
		}
	}
// ------------------------------------------------------------------------------------
void PrintTitle (HANDLE console)
	{
	BRIGHT_WHITE;
	_tprintf(_T("\n")); 
	_tprintf(_T("   -WiiYourself!- "));
	WHITE; _tprintf(		   _T("library Demo:   "));
	CYAN;  _tprintf(						   _T("| (c) "));
	BRIGHT_CYAN;  _tprintf(							 _T("gl"));
	BRIGHT_PURPLE;_tprintf(                            _T("."));
	BRIGHT_CYAN;  _tprintf(								_T("tter"));
	CYAN; _tprintf(										    _T(" 2007-10\n")
			 _T("                    v") WIIYOURSELF_VERSION_STR
										 _T("         | http://gl.tter.org\n"));
	CYAN;_tprintf(_T(" ______________________________________________________________________\n\n\n"));
	}
// ------------------------------------------------------------------------------------
int _tmain ()
	{
	SetConsoleTitle(_T("- WiiYourself! - Demo: "));
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	// write the title
	PrintTitle(console);

	// let's load a couple of samples:
	wiimote_sample sine_sample, daisy_sample;

	// one .raw (just to demonstrate it)
	if(!wiimote::Load16BitMonoSampleRAW(_T("1kSine16 (3130).raw"),
										true, FREQ_3130HZ, sine_sample)) {
		_tprintf(_T("\r  ** can't find 'Sine16 (3130).raw' - (sample won't work!) **"));
		Beep(100, 1000);
		Sleep(3000);
		_tprintf(_T("\r") BLANK_LINE);
		}
	// and one (more convenient) .wav 
	if(!wiimote::Load16bitMonoSampleWAV(_T("Daisy16 (3130).wav"), daisy_sample)) {
		_tprintf(_T("\r  ** can't find 'Daisy16 (3130).wav' - (sample won't work!) **"));
		Beep(100, 1000);
		Sleep(3000);
		_tprintf(_T("\r") BLANK_LINE);
		}

	// create a wiimote object
	wiimote remote;
	
	// in this demo we use a state-change callback to get notified of
	//  extension-related events, and polling for everything else
	// (note you don't have to use both, use whatever suits your app):
	remote.ChangedCallback		= on_state_change;
	//  notify us only when the wiimote connected sucessfully, or something
	//   related to extensions changes
	remote.CallbackTriggerFlags = (state_change_flags) (CONNECTED |
													    EXTENSION_CHANGED |
														MOTIONPLUS_CHANGED);
reconnect:
	COORD pos = { 0, 6 };
	SetConsoleCursorPosition(console, pos);

	// try to connect the first available wiimote in the system
	//  (available means 'installed, and currently Bluetooth-connected'):
	WHITE; _tprintf(_T("  Looking for a Wiimote     "));
	   
	static const TCHAR* wait_str[] = { _T(".  "), _T(".. "), _T("...") };
	unsigned count = 0;
	while(!remote.Connect(wiimote::FIRST_AVAILABLE)) {
		_tprintf(_T("\b\b\b\b%s "), wait_str[count%3]);
		count++;
#ifdef USE_BEEPS_AND_DELAYS
		Beep(500, 30); Sleep(1000);
#endif
		}

	// connected - light all LEDs
	remote.SetLEDs(0x0f);
	BRIGHT_CYAN; _tprintf(_T("\b\b\b\b... connected!"));
	if(remote.IsBalanceBoard()) {
		BRIGHT_WHITE;_tprintf(_T("  (Balance Board)"));
		}

#ifdef USE_BEEPS_AND_DELAYS
	Beep(1000, 300); Sleep(2000);
#endif

	COORD cursor_pos = { 0, 6 };

#ifdef LOOK_FOR_ADDITIONAL_WIIMOTES
	// try to connect any additional wiimotes (just to show the code)
	_tprintf(_T("\n\n"));

	wiimote  *extra_motes [7] = { NULL }; // 7 should cover it
	unsigned detected		  = 0;
	while(detected < 7)
		{
		wiimote *next = new wiimote;
		if(!next->Connect(wiimote::FIRST_AVAILABLE))
			break;
		extra_motes[detected++] = next;
		
		WHITE		; _tprintf(_T("   also found wiimote "));
		BRIGHT_GREEN; _tprintf(_T("%u"), detected+1);
		if(next->IsBalanceBoard()) {
			WHITE; _tprintf(_T("  (Balance Board)"));
			}
		_tprintf(_T("\n\n"));

# ifdef USE_BEEPS_AND_DELAYS
		Beep(1000 + (detected*100), 100);
		Sleep(500);
# endif
		}
	WHITE; _tprintf( ((detected == 7)? _T("     (can't detect any more).") :
									   _T("    (no more found).")) );
# ifdef USE_BEEPS_AND_DELAYS
	Sleep(2000);
# endif
	// clean up
	for(unsigned index=0; index<detected; index++)
		delete extra_motes[index];
	
	SetConsoleCursorPosition(console, cursor_pos);
#endif // LOOK_FOR_ADDITIONAL_WIIMOTES

	// print the button event instructions:
	BRIGHT_WHITE;
	_tprintf(_T("\r  -- TRY: B = rumble, A = square, 1 = sine, 2 = daisy, Home = Exit --\n"));

	// (stuff for animations)
	DWORD	 last_rumble_time = timeGetTime(); // for rumble text animation
	DWORD    last_led_time    = timeGetTime(); // for led         animation
	bool	 rumble_text	  = true;
	unsigned lit_led          = 0;	
	
	// display the wiimote state data until 'Home' is pressed:
	while(!remote.Button.Home())// && !GetAsyncKeyState(VK_ESCAPE))
		{
		// IMPORTANT: the wiimote state needs to be refreshed each pass
		while(remote.RefreshState() == NO_CHANGE)
			Sleep(1); // // don't hog the CPU if nothing changed

		cursor_pos.Y = 8;
		SetConsoleCursorPosition(console, cursor_pos);

		// did we loose the connection?
		if(remote.ConnectionLost())
			{
			BRIGHT_RED; _tprintf(
				_T("   *** connection lost! ***                                          \n")
				BLANK_LINE BLANK_LINE BLANK_LINE BLANK_LINE BLANK_LINE BLANK_LINE
				BLANK_LINE BLANK_LINE BLANK_LINE BLANK_LINE BLANK_LINE BLANK_LINE
				BLANK_LINE BLANK_LINE BLANK_LINE);
			Beep(100, 1000);
			Sleep(2000);
			COORD pos = { 0, 6 };
			SetConsoleCursorPosition(console, pos);
			_tprintf(BLANK_LINE BLANK_LINE BLANK_LINE);
			goto reconnect;
			}

		// rumble if 'B' (trigger) is pressed
		remote.SetRumble(remote.Button.B());

		// TEMP: Minus button disables MotionPlus (if connected) to allow its
		//        own extension port to work
		static bool last_minus = false;
		if(remote.Button.Minus() && !last_minus && 
		   (remote.ExtensionType == wiimote_state::MOTION_PLUS))
			remote.DisableMotionPlus();
		last_minus = remote.Button.Minus();

		// actions for buttons just pressed/released:
		static bool last_A = false, last_One = false, last_Two = false;

		#define ON_PRESS_RELEASE(button, pressed_action, released_action)	\
			{ bool pressed = remote.Button.button();						\
			  if(pressed)													\
			 	{				   /* just pressed? */						\
				if(!last_##button) pressed_action;							\
				}															\
			else if(last_##button) /* just released */						\
				released_action;											\
			/* remember the current button state for next time */			\
			last_##button = pressed; }
			
		//  play audio whilst certain buttons are held
		if(!remote.IsBalanceBoard()) {
			ON_PRESS_RELEASE(  A, remote.PlaySquareWave(FREQ_3130HZ, 0x20), 
								  remote.EnableSpeaker (false));
			ON_PRESS_RELEASE(One, remote.PlaySample	   (sine_sample),
								  remote.EnableSpeaker (false));
			ON_PRESS_RELEASE(Two, remote.PlaySample	   (daisy_sample),
								  remote.EnableSpeaker (false));
			}

		// Battery level:
		CYAN; _tprintf(_T("  Battery: "));
		// (the green/yellow colour ranges are rough guesses - my wiimote
		//  with rechargeable battery pack fails at around 15%)
		(remote.bBatteryDrained	    )? BRIGHT_RED   :
		(remote.BatteryPercent >= 30)? BRIGHT_GREEN : BRIGHT_YELLOW;
		_tprintf(_T("%3u%%   "), remote.BatteryPercent);

		DWORD current_time = timeGetTime();

		// LEDs:
		//  animate them every second
		if((current_time - last_led_time) >= 1000) {
			remote.SetLEDs((BYTE)(1<<lit_led));
			lit_led		  = (++lit_led) % 4;
			last_led_time = timeGetTime();
			}

		CYAN; _tprintf(_T("LEDs: ")); WHITE; _tprintf(_T("["));
		for(unsigned led=0; led<4; led++)
			{
			if(remote.LED.Lit(led)) {
				BRIGHT_CYAN; _tprintf(_T("*"));
				}
			else{
				WHITE      ; _tprintf(_T("-"));//_T("%c"), '0'+led);
				}
			}

		// Rumble
		WHITE; _tprintf(_T("] "));
		if(remote.bRumble) {
			BRIGHT_WHITE; _tprintf(rumble_text? _T(" RUMBLE") : _T("RUMBLE "));
			// animate the text
			if((current_time - last_rumble_time) >= 110) {
				rumble_text		 = !rumble_text;
				last_rumble_time = current_time;
				}
			}
		else
			_tprintf(_T("       "));

		// Output method:
	    CYAN; _tprintf( _T("        using %s\n"), (remote.IsUsingHIDwrites()?
											   _T("HID writes") : _T("WriteFile()")));
		
		// 'Unique' IDs (not guaranteed to be unique, check the variable
		//  defintion for details)
		CYAN  ; _tprintf(_T("       ID: "));
		YELLOW; _tprintf(_T("%I64u")  , remote.UniqueID);
#ifdef ID2_FROM_DEVICEPATH		// (see comments in header)
		CYAN;  _tprintf(_T("   ID2: "));
		WHITE; _tprintf(_T("%I64u\n"), remote.UniqueID2);
#else
		_tprintf(_T("\n"));
#endif

		// Buttons:
		CYAN; _tprintf(_T("  Buttons: ")); WHITE; _tprintf(_T("["));
		for(unsigned bit=0; bit<16; bit++)
			{
			WORD mask = (WORD)(1 << bit);
			// skip unused bits
			if((wiimote_state::buttons::ALL & mask) == 0)
				continue;

			const TCHAR* button_name = wiimote::ButtonNameFromBit[bit];
			bool		 pressed	 = ((remote.Button.Bits & mask) != 0);
			if(bit > 0) {
				CYAN; _tprintf(_T("|")); // seperator
				}
			if(pressed) {
				BRIGHT_WHITE; _tprintf(_T("%s")  , button_name);
				}
			else{
				WHITE       ; _tprintf(_T("%*s"), _tcslen(button_name), _T(""));
				}
			}
		WHITE; _tprintf(_T("]\n"));

		// Acceleration:
		CYAN ; _tprintf(_T("    Accel:"));
		remote.IsBalanceBoard()? RED : WHITE;
		_tprintf(_T("  X %+2.3f  Y %+2.3f  Z %+2.3f  \n"),
					remote.Acceleration.X,
					remote.Acceleration.Y,
					remote.Acceleration.Z);
	
		// Orientation estimate (shown red if last valid update is aging):
		CYAN ; _tprintf(_T("   Orient:"));
		remote.IsBalanceBoard()? RED : WHITE;
		_tprintf(_T("  UpdateAge %3u  "), remote.Acceleration.Orientation.UpdateAge);
		
		//  show if the last orientation update is considered out-of-date
		//   (using an arbitrary threshold)
		if(remote.Acceleration.Orientation.UpdateAge > 10)
			RED;
			
		_tprintf(_T("Pitch:%4ddeg  Roll:%4ddeg  \n")
			     _T("                           (X %+.3f  Y %+.3f  Z %+.3f)      \n"),
				 (int)remote.Acceleration.Orientation.Pitch,
				 (int)remote.Acceleration.Orientation.Roll ,
				 remote.Acceleration.Orientation.X,
				 remote.Acceleration.Orientation.Y,
  				 remote.Acceleration.Orientation.Z);
				
		// IR:
		CYAN ; _tprintf(_T("       IR:"));
		remote.IsBalanceBoard()? RED : WHITE;
		_tprintf(_T("  Mode %s  "),
			((remote.IR.Mode == wiimote_state::ir::OFF     )? _T("OFF  ") :
			 (remote.IR.Mode == wiimote_state::ir::BASIC   )? _T("BASIC") :
			 (remote.IR.Mode == wiimote_state::ir::EXTENDED)? _T("EXT. ") :
															  _T("FULL ")));
		// IR dot sizes are only reported in EXTENDED IR mode (FULL isn't supported yet)
		bool dot_sizes = (remote.IR.Mode == wiimote_state::ir::EXTENDED);

		for(unsigned index=0; index<4; index++)
			{
			wiimote_state::ir::dot &dot = remote.IR.Dot[index];
			
			if(!remote.IsBalanceBoard()) WHITE;
			_tprintf(_T("%u: "), index);

			if(dot.bVisible) {
				WHITE; _tprintf(_T("Seen       "));
				}
			else{
				RED  ; _tprintf(_T("Not seen   "));
				}

			_tprintf(_T("Size"));
			if(dot_sizes)
				 _tprintf(_T("%3d "), dot.Size);
			else{
				RED; _tprintf(_T(" n/a"));
				if(dot.bVisible) WHITE;
				}

			_tprintf(_T("  X %.3f  Y %.3f\n"), dot.X, dot.Y);
			
			if(index < 3)
				_tprintf(_T("                        "));
			}

		// Speaker:
		CYAN ; _tprintf(_T("  Speaker:"));
		remote.IsBalanceBoard()? RED : WHITE;
		_tprintf(_T("  %s | %s    "), (remote.Speaker.bEnabled? _T("On ") :
															    _T("Off")),
									  (remote.Speaker.bMuted  ? _T("Muted") :
																_T("     ")));
		if(!remote.Speaker.bEnabled || remote.Speaker.bMuted)
			RED;
		else//if(remote.IsPlayingAudio()) BRIGHT_WHITE; else WHITE;
			WHITE;
		_tprintf(_T("Frequency %4u Hz   Volume 0x%02x\n"),
				 wiimote::FreqLookup[remote.Speaker.Freq],
				 remote.Speaker.Volume);
		
		// -- Extensions --:
		CYAN ; _tprintf(_T("__________\n  Extnsn.:  "));
		switch(remote.ExtensionType)
			{
			case wiimote_state::NONE:
				{
				RED;
				_tprintf(_T("None                                                             \n"));
				_tprintf(BLANK_LINE BLANK_LINE BLANK_LINE);
				}
				break;
			
			case wiimote_state::PARTIALLY_INSERTED:
				{
				BRIGHT_RED;
				_tprintf(_T("Partially Inserted                                               \n"));
				_tprintf(BLANK_LINE BLANK_LINE BLANK_LINE);
				}
				break;
			
			// -- Nunchuk --
			case wiimote_state::NUNCHUK:
				{
				BRIGHT_WHITE; _tprintf(_T("Nunchuk   "));

				// Buttons:
				CYAN		; _tprintf(_T("Buttons: ")); WHITE; _tprintf(_T("["));
				BRIGHT_WHITE; _tprintf(remote.Nunchuk.C? _T("C") : _T(" "));
				CYAN		; _tprintf(_T("|"));
				BRIGHT_WHITE; _tprintf(remote.Nunchuk.Z? _T("Z") : _T(" "));
				WHITE		; _tprintf(_T("]   "));
				// Joystick:
				CYAN		; _tprintf(_T("Joystick:  "));
				WHITE		; _tprintf(_T("X %+2.3f  Y %+2.3f\n"),
									   remote.Nunchuk.Joystick.X,
									   remote.Nunchuk.Joystick.Y);
				// Acceleration:
				CYAN		; _tprintf(_T("    Accel:"));
				WHITE		; _tprintf(_T("  X %+2.3f  Y %+2.3f  Z %+2.3f  \n"),
									   remote.Nunchuk.Acceleration.X,
									   remote.Nunchuk.Acceleration.Y,
									   remote.Nunchuk.Acceleration.Z);
				
				// Orientation estimate (shown red if last valid update is aging):
				CYAN		; _tprintf(_T("   Orient:"));
				WHITE		; _tprintf(_T("  UpdateAge %3u  "),
								remote.Nunchuk.Acceleration.Orientation.UpdateAge);
				//  show if the last orientation update is aging
				if(remote.Nunchuk.Acceleration.Orientation.UpdateAge > 10)
					RED;
				_tprintf(_T("Pitch %4ddeg  Roll %4ddeg  \n")
					     _T("                           (X %+.2f  Y %+.2f  Z %+.2f)      \n"),
						 (int)remote.Nunchuk.Acceleration.Orientation.Pitch,
						 (int)remote.Nunchuk.Acceleration.Orientation.Roll ,
						 remote.Nunchuk.Acceleration.Orientation.X,
						 remote.Nunchuk.Acceleration.Orientation.Y,
  						 remote.Nunchuk.Acceleration.Orientation.Z);
				}
				break;

			// -- Classic Controller --
			case wiimote_state::CLASSIC:
			case wiimote_state::GH3_GHWT_GUITAR:
			case wiimote_state::GHWT_DRUMS:
				{
				BRIGHT_WHITE;
				// the Guitar Hero controller is just a classic controller with
				//  another ID
				if(remote.ExtensionType == wiimote_state::CLASSIC)
					_tprintf(_T("Classic Controller   "));
				else if(remote.ExtensionType == wiimote_state::GH3_GHWT_GUITAR)
					_tprintf(_T("GH3/GHWT Guitar      "));
				else
					_tprintf(_T("GHWT Drums           "));
				
				// L: Joystick/Trigger
				WHITE; _tprintf(_T("L:  "));
				CYAN ; _tprintf(_T("Joy "));
				WHITE; _tprintf(_T("X %+2.3f  Y %+2.3f  "),
								remote.ClassicController.JoystickL.X,
								remote.ClassicController.JoystickL.Y);
				CYAN ; _tprintf(_T("Trig "));
				WHITE; _tprintf(_T("%+2.3f\n"),
								remote.ClassicController.TriggerL);
				// R: Joystick/Trigger
				WHITE; _tprintf(_T("                                 R:  "));
				CYAN ; _tprintf(_T("Joy "));
				WHITE; _tprintf(_T("X %+2.3f  Y %+2.3f  "),
								remote.ClassicController.JoystickR.X,
								remote.ClassicController.JoystickR.Y);
				CYAN ; _tprintf(_T("Trig "));
				WHITE; _tprintf(_T("%+2.3f\n"),
								remote.ClassicController.TriggerR);

				// Buttons:
				CYAN; _tprintf(_T("  Buttons: ")); WHITE; _tprintf(_T("["));
				for(unsigned bit=0; bit<16; bit++)
					{
					WORD mask = (WORD)(1 << bit);
					// skip unused bits
					if((wiimote_state::classic_controller::buttons::ALL & mask) == 0)
						continue;

					const TCHAR* button_name = wiimote::ClassicButtonNameFromBit[bit];
					const TCHAR* seperator	 = (bit==0)? _T("") : _T("|");
					bool		 pressed	 = ((remote.ClassicController.Button.Bits & mask) != 0);
					CYAN; _tprintf(seperator);
					if(pressed) {
						BRIGHT_WHITE; _tprintf(_T("%s")  , button_name);
						}
					else{
						WHITE	; _tprintf(_T("%*s"), _tcslen(button_name), _T(""));
						}
					}
				WHITE; _tprintf(_T("]"));
				}
				break;

			case wiimote_state::BALANCE_BOARD:
				{
				BRIGHT_WHITE; _tprintf(_T("Balance Board"));

				// Weights:
				CYAN ; _tprintf(_T("   Weight: "));
				WHITE; _tprintf(_T("TL    "));
					   _tprintf(_T("%6.2f"), remote.BalanceBoard.Kg.TopL);
				CYAN ; _tprintf(_T(" kg")); WHITE; ;_tprintf(_T("  TR "));
					   _tprintf(_T("%6.2f"), remote.BalanceBoard.Kg.TopR);
				CYAN;  _tprintf(_T(" kg\n")); WHITE;
					   _tprintf(_T("                                    BL    "));
					   _tprintf(_T("%6.2f"), remote.BalanceBoard.Kg.BottomL);
				CYAN;  _tprintf(_T(" kg")); WHITE; _tprintf(_T("  BR "));
					   _tprintf(_T("%6.2f"), remote.BalanceBoard.Kg.BottomR);
				CYAN;  _tprintf(_T(" kg \n")); WHITE;
					   _tprintf(_T("                                    Total "));
					   _tprintf(_T("%6.2f"), remote.BalanceBoard.Kg.Total);
				CYAN;  _tprintf(_T(" kg"));
				}
				break;

			case wiimote_state::MOTION_PLUS:
				{
				BRIGHT_WHITE; _tprintf(_T("Motion Plus"));

				CYAN ; _tprintf(_T("    Raw: "));
				WHITE; _tprintf(_T("Yaw: %04hx  ")   , remote.MotionPlus.Raw.Yaw);
				WHITE; _tprintf(_T("Pitch: %04hx  ") , remote.MotionPlus.Raw.Pitch);
				WHITE; _tprintf(_T("Roll: %04hx  \n"), remote.MotionPlus.Raw.Roll);
				CYAN ; _tprintf(_T("                         Float: "));
				WHITE; _tprintf(_T("  %8.3fdeg")     , remote.MotionPlus.Speed.Yaw);
				WHITE; _tprintf(_T("  %8.3fdeg")   , remote.MotionPlus.Speed.Pitch);
				WHITE; _tprintf(_T(" %8.3fdeg\n")   , remote.MotionPlus.Speed.Roll);
				_tprintf(BLANK_LINE BLANK_LINE);
//Sleep(500);
				}
				break;
			}
  		}


	// disconnect (auto-happens on wiimote destruction anyway, but let's play nice)
	remote.Disconnect();
	Beep(1000, 200);

	BRIGHT_WHITE; // for automatic 'press any key to continue' msg
	CloseHandle(console);

	return 0;
	}
// ------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------
