import wiimote
import time
import sys

class mywiimote(wiimote.wiimote):
    def ChangedNotifier(self, flags, new_state):
        # CAVEAT! If *ANY* exception occurs in ChangedNotifier,
        # the application will terminate. So, be nice and don't crash.
        self.buttons = []
        for bit in range(wiimote.TOTAL_BUTTON_BITS):
            mask = 1<<bit
            # skip unused bits
            if wiimote.wiimote_state.buttons.ALL & mask == 0:
                continue
            button_name = remote.GetButtonNameFromBit(bit)
            pressed = bool(new_state.Button.Bits & mask)
            if pressed:
                self.buttons.append(button_name)

remote=mywiimote()
while not remote.Connect(remote.FIRST_AVAILABLE):
    time.sleep(0.1)

print "Connected!"
remote.SetReportType(remote.IN_BUTTONS_ACCEL_IR)
remote.buttons = []
remote.prev_buttons = []
remote.SetLEDs(0xa)

while 1:
    # spin to allow events to be generated
    time.sleep(0.1)
    if remote.buttons != remote.prev_buttons:
        if remote.buttons:
            print " ".join(remote.buttons)
        else:
            print "None"
    remote.prev_buttons = remote.buttons
