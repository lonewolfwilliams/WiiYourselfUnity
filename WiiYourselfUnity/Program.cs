using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace WiiYourselfWrapperHarness
{
    class Program
    {
        
        static void Main(string[] args)
        {
            var bridge = new WiiMoteBridge();
            bridge.TryConnect();

            while (true)
            {
                Console.WriteLine("\n esc to exit 1 to 4 to light up wiimote led:");
                ConsoleKeyInfo key = Console.ReadKey(false);
                if (key.Key == ConsoleKey.Escape)
                {
                    bridge.Disconnect();
                    return;
                }
                else if (key.Key == ConsoleKey.D1)
                {
                    bridge.ToggleLED(WiiMoteBridge.PlayerLED.playerOne);
                }
                else if (key.Key == ConsoleKey.D2)
                {
                    bridge.ToggleLED(WiiMoteBridge.PlayerLED.playerTwo);
                }
                else if (key.Key == ConsoleKey.D3)
                {
                    bridge.ToggleLED(WiiMoteBridge.PlayerLED.playerThree);
                }
                else if (key.Key == ConsoleKey.D4)
                {
                    bridge.ToggleLED(WiiMoteBridge.PlayerLED.playerFour);
                }
            }
        }
    }
}
