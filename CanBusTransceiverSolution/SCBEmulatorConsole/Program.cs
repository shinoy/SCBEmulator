using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CommandLine;
using CommandLine.Text;
using CanBusChannelNS;
using SCBProtocalWrapper;
using PositionerSimulationNS;
using System.Threading;
using PCUSimulationNS;

namespace SCBEmulatorConsole
{
    class WSDOption
    {
        [Option('x', "xposition", DefaultValue = -8192.00, HelpText = "WSD Axis Controller X Position Value")]
        public double Xpos { get; set; }

        [Option('y', "yposition", DefaultValue = -8192.00, HelpText = "WSD Axis Controller Y Position Value")]
        public double Ypos { get; set; }

        [Option('z', "zposition", DefaultValue = -8192.00, HelpText = "WSD Axis Controller Z Position Value")]
        public double ZPos { get; set; }

        [Option('t', "tiltposition", DefaultValue = -3276.0, HelpText = "WSD Axis Controller Tilt Position Value")]
        public double TiltPos { get; set; }

        [Option('a', "angposition", DefaultValue = -3276.0, HelpText = "WSD Axis Controller Angulation Position Value")]
        public double AngPos { get; set; }

        [HelpOption]
        public string GetUsage()
        {
            // WSD option description
            var usage = new StringBuilder();
            usage.AppendLine("The command WSD move will move wall stand to designated position");
            usage.AppendLine("wsd move [-x xpos] [-y ypos] [-z zpos] [-t tiltpos] [-a angpos]");
            return usage.ToString();
        }
    }

    class OTCOption
    {
        [Option('x', "xposition",DefaultValue = -8192.00, HelpText = "OTC Axis Controller X Position Value")]
        public double Xpos { get; set; }

        [Option('y', "yposition",DefaultValue = -8192.00, HelpText = "OTC Axis Controller Y Position Value")]
        public double Ypos { get; set; }

        [Option('z', "zposition",DefaultValue = -8192.00, HelpText = "OTC Axis Controller Z Position Value")]
        public double ZPos { get; set; }

        [Option('a', "aposition", DefaultValue = -3276.0, HelpText = "OTC Axis Controller a Position Value")]
        public double APos { get; set; }

        [Option('b', "bposition", DefaultValue = -3276.0, HelpText = "OTC Axis Controller b Position Value")]
        public double BPos { get; set; }

        [HelpOption]
        public string GetUsage()
        {
            // OTC option description
            var usage = new StringBuilder();
            usage.AppendLine("The command OTC move will move OTC to designated position");
            usage.AppendLine("otc move [-x xpos] [-y ypos] [-z zpos] [-a apos] [-b bpos]");
            return usage.ToString();
        }
    }

    class TBLOption
    {
        [Option('x', "xposition", DefaultValue = -8192.00,HelpText = "TBL Axis Controller X Position Value")]
        public double Xpos { get; set; }

        [Option('y', "yposition",DefaultValue = -8192.00, HelpText = "TBL Axis Controller Y Position Value")]
        public double Ypos { get; set; }

        [Option('z', "zposition",DefaultValue = -8192.00, HelpText = "TBL Axis Controller Z Position Value")]
        public double ZPos { get; set; }

        [HelpOption]
        public string GetUsage()
        {
            // OTC option description
            var usage = new StringBuilder();
            usage.AppendLine("The command TBL move will move TBL to designated position");
            usage.AppendLine("tbl move [-x xpos] [-y ypos] [-z zpos] [-a apos] [-b bpos]");
            return usage.ToString();
        }
    }

    class Program
    {
        private static CanBusChannelCls bus = new CanBusChannelCls();

        private static OTCXYZControllerCLS otcxyz = new OTCXYZControllerCLS(bus);

        private static WSDControllerCLS  wsd = new WSDControllerCLS(bus);

        private static TBLControllerCLS  tbl = new TBLControllerCLS(bus);

        private static CollimatorCLS col = new CollimatorCLS(bus);

        private static PCUSimulator pcu = new PCUSimulator("COM18", "COM19");

        private static bool busStarted = false;

        static void Main(string[] args)
        {
            var startUpPrompt = new StringBuilder();
            startUpPrompt.AppendLine("Virtual X-RAY Lab System version 1.0");
            Console.WriteLine(startUpPrompt);
            try
            {
                bus.Connect();
                pcu.BuckyStartEvent += tbl.BuckyStartEventHandler;
                pcu.Connect();
            }
            catch (Exception ex)
            {
                Console.WriteLine("Make sure the emulator hardware is connected with control PC!");
                Console.WriteLine("Error: " + ex.ToString());
            }
            RunEmulatorCliSystem();
           
        }

        public static string[] SplitArray(string[] Source, int StartIndex, int EndIndex)
        {
            try
            {
                string[] result = new string[EndIndex - StartIndex + 1];
                for (int i = 0; i <= EndIndex - StartIndex; i++) result[i] = Source[i + StartIndex];
                return result;
            }
            catch (IndexOutOfRangeException ex)
            {
                throw new Exception(ex.Message);
            }
        }

        static void RunEmulatorCliSystem()
        {
            while (true)
            {
                Console.Write("lab v1.0>");
                String command = Console.ReadLine();
                if (command == "exit")
                {
                    return;
                }
                else
                {
                    ParseSysCmd(command);
                }
            }
        }

        static void ParseSysCmd(string cmd)
        {
            string[] cmdArray = cmd.Split(new char[]{' '}, StringSplitOptions.RemoveEmptyEntries);
            if (cmdArray.Length < 2)
            {
                Console.WriteLine("Bad Command");
                return;
            }
            switch (cmdArray[0])
            { 
                case "wsd":
                    switch (cmdArray[1])
                    { 
                        case "powup":
                            if (busStarted)
                            {
                                if (wsd.OP_MODE != ControlOpMode.NoInit)
                                {
                                    Console.WriteLine("WSD is already started,please stop first");
                                }
                                else
                                {
                                    try
                                    {
                                        wsd.PowerUp();
                                        Console.WriteLine("WSD is started successfully");
                                    }
                                    catch (Exception ex)
                                    {
                                        Console.WriteLine("WSD could not start");
                                        Console.WriteLine("WSD ERROR: " + ex.Message);
                                    }
                                }
                            }
                            else
                            {
                                Console.WriteLine("Need to start bus before start WSD");
                            }
                            break;
                        case "powoff":
                            if (wsd.OP_MODE != ControlOpMode.NoInit)
                            {
                                try
                                {
                                    wsd.PowerOff();
                                    Console.WriteLine("WSD is stopped successfully");
                                }
                                catch (Exception ex)
                                {
                                    Console.WriteLine("WSD could not stop");
                                    Console.WriteLine("WSD ERROR: " + ex.Message);
                                }
                            }
                            else
                            {
                                Console.WriteLine("WSD is already stopped,please start firstly");
                            }
                            break;
                        case "autocenter":
                            wsd.keypad.AUTO_CENTER_SW = true;
                            Thread.Sleep(500);
                            Console.Write("Moving.");
                            while ((wsd.OP_MODE != ControlOpMode.Idle) || (otcxyz.OP_MODE != ControlOpMode.Idle) || (tbl.OP_MODE != ControlOpMode.Idle) || (otcxyz.AB_OPMODE != ControlOpMode.Idle))
                            {
                                Console.Write(" .");
                                Thread.Sleep(500);
                                Console.Write(" .");
                                Thread.Sleep(500);
                                Console.Write(" .");
                            }
                            Console.WriteLine("");
                            Console.WriteLine("Auto Center Finished");
                            wsd.keypad.AUTO_CENTER_SW = false;
                            break;
                        case "autopos":
                            wsd.keypad.AUTO_POS_SW = true;
                            Thread.Sleep(500);
                            Console.Write("Moving.");
                            while ((wsd.OP_MODE != ControlOpMode.Idle) || (otcxyz.OP_MODE != ControlOpMode.Idle) || (tbl.OP_MODE != ControlOpMode.Idle) || (otcxyz.AB_OPMODE != ControlOpMode.Idle))
                            {
                                Console.Write(" .");
                                Thread.Sleep(500);
                                Console.Write(" .");
                                Thread.Sleep(500);
                                Console.Write(" .");
                            }
                            Console.WriteLine("");
                            Console.WriteLine("Auto Position Finished");
                            wsd.keypad.AUTO_POS_SW = false;
                            break;
            
                        case "getstat":
                            Console.WriteLine("WSD Status: " + wsd.OP_MODE.ToString());
                            break;
                        case "getpos":
                            Console.WriteLine("WSD Position: " + "X-" + wsd.X_POS.ToString() + " Y-" + wsd.Y_POS.ToString() + " Z-" + wsd.Z_POS.ToString() + " Tilt-" + wsd.TILT_POS.ToString() + " Ang-" + wsd.ANG_POS.ToString());
                            break;
                        case "move":
                            if (cmdArray.Length < 3)
                            {
                                Console.WriteLine("The command wsd move will move WSD to designated position");
                                Console.WriteLine("wsd move [-x xpos] [-y ypos] [-z zpos] [-t tiltpos] [-a angpos]");
                                break;
                            }
                            var options = new WSDOption();
                            var parser = new CommandLine.Parser(with => with.HelpWriter = Console.Out);

                             if (parser.ParseArgumentsStrict(SplitArray(cmdArray,2,cmdArray.Length-1), options, () => Console.WriteLine()))
                              {
                                  try
                                  {
                                      if (options.Xpos != -8192.00)
                                      {
                                          wsd.XRelBtnPress();
                                          wsd.X_POS = options.Xpos;
                                          wsd.XRelBtnRelease();
                                      }
                                      if (options.ZPos != -8192.00)
                                      {
                                          wsd.ZRelBtnPress();
                                          wsd.Z_POS = options.ZPos;
                                          wsd.ZRelBtnRelease();
                                      }
                                      Thread.Sleep(500);
                                      Console.WriteLine("WSD Move Finished!");
                                  }
                                  catch (Exception ex)
                                  {
                                      Console.WriteLine("WSD Move Error: " + ex.Message);
                                  }
                              }
                            break;
                        default:
                            Console.WriteLine("unknown WSD command,please make sure the command format is correct!");
                            break;
                    }
                    break;
                case "otc":
                    switch (cmdArray[1])
                    {
                        case "powup":
                            if (busStarted)
                            {
                                if ((otcxyz.OP_MODE != ControlOpMode.NoInit) || (otcxyz.AB_OPMODE != ControlOpMode.NoInit))
                                {
                                    Console.WriteLine("OTC is already started,please stop first");
                                }
                                else
                                {
                                    try
                                    {
                                        otcxyz.PowerUp();
                                        Console.WriteLine("OTC is started successfully");
                                    }
                                    catch (Exception ex)
                                    {
                                        Console.WriteLine("OTC could not start");
                                        Console.WriteLine("OTC ERROR: " + ex.Message);
                                    }
                                }
                            }
                            else
                            {
                                Console.WriteLine("Need to start bus before start OTC");
                            }
                            break;
                        case "powoff":
                            if (otcxyz.OP_MODE != ControlOpMode.NoInit)
                            {
                                try
                                {
                                    otcxyz.PowerOff();
                                    Console.WriteLine("OTC is stopped successfully");
                                }
                                catch (Exception ex)
                                {
                                    Console.WriteLine("OTC could not stop");
                                    Console.WriteLine("OTC ERROR: " + ex.Message);
                                }
                            }
                            else
                            {
                                Console.WriteLine("OTC is already stopped,please start firstly");
                            }
                            break;
                        case "getstat":
                            Console.WriteLine("OTC XYZ Status: " + otcxyz.OP_MODE.ToString());
                            Console.WriteLine("OTC AB  Status: " + otcxyz.AB_OPMODE.ToString());
                            break;
                        case "getpos":
                            Console.WriteLine("OTC Position: " + "X-" + otcxyz.X_POS.ToString() + " Y-" + otcxyz.Y_POS.ToString() + " Z-" + otcxyz.Z_POS.ToString() + " A-" + otcxyz.A_POS.ToString() + " B" + otcxyz.B_POS.ToString());
                            break;
                        case "xyzmove":
                            {
                                if (cmdArray.Length < 3)
                                {
                                    Console.WriteLine("The command otc xyzmove will move otc xyz to designated position");
                                    Console.WriteLine("otc xyzmove [-x xpos] [-y ypos] [-z zpos]");
                                    break;
                                }
                                var options = new OTCOption();
                                var parser = new CommandLine.Parser(with => with.HelpWriter = Console.Out);
                                if (parser.ParseArgumentsStrict(SplitArray(cmdArray, 2, cmdArray.Length - 1), options, () => Console.WriteLine()))
                                {
                                    try
                                    {
                                        otcxyz.XYZRelBtnPress();
                                        if (options.Xpos != -8192.00)
                                        {
                                            otcxyz.X_POS = options.Xpos;
                                        }
                                        if (options.Ypos != -8192.00)
                                        {
                                            otcxyz.Y_POS = options.Ypos;
                                        }
                                        if (options.ZPos != -8192.00)
                                        {
                                            otcxyz.Z_POS = options.ZPos;
                                        }
                                        otcxyz.XYZRelBtnRelease();
                                        Thread.Sleep(500);
                                        Console.WriteLine("OTC xyzmove Finished!");
                                    }
                                    catch (Exception ex)
                                    {
                                        Console.WriteLine("OTC xyzmove Error: " + ex.Message);
                                    }
                                }
                            }
                            break;
                        case "abmove":
                            {
                                if (cmdArray.Length < 3)
                                {
                                    Console.WriteLine("The command otc  abmove will move otc ab to designated position");
                                    Console.WriteLine("otc abmove [-a apos] [-b bpos]");
                                    break;
                                }
                                var options = new OTCOption();
                                var parser = new CommandLine.Parser(with => with.HelpWriter = Console.Out);
                                if (parser.ParseArgumentsStrict(SplitArray(cmdArray, 2, cmdArray.Length - 1), options, () => Console.WriteLine()))
                                {
                                    try
                                    {
                                   
                                        if (options.APos != -3276.0)
                                        {
                                            otcxyz.ARelBtnPress();
                                            otcxyz.A_POS = options.APos;
                                            otcxyz.ARelBtnRelease();
                                        }

                                        if (options.BPos != -3276.0)
                                        {
                                            otcxyz.BRelBtnPress();
                                            otcxyz.B_POS = options.BPos;
                                            otcxyz.BRelBtnRelease();
                                        }
                                        Thread.Sleep(500);
                                        Console.WriteLine("OTC AB Move Finished!");
                                    }
                                    catch (Exception ex)
                                    {
                                        Console.WriteLine("OTC AB Move Error: " + ex.Message);
                                    }
                                }
                            }
                            break;
                        default:
                            Console.WriteLine("unknown OTC command,please make sure the command format is correct!");
                            break;
                    }
                    break;
                case "tbl":
                    switch (cmdArray[1])
                    {
                        case "powup":
                            if (busStarted)
                            {
                                if (tbl.OP_MODE != ControlOpMode.NoInit)
                                {
                                    Console.WriteLine("TBL is already started,please stop first");
                                }
                                else
                                {
                                    try
                                    {
                                        tbl.PowerUp();
                                        Console.WriteLine("TBL is started successfully");
                                    }
                                    catch (Exception ex)
                                    {
                                        Console.WriteLine("TBL could not start");
                                        Console.WriteLine("TBL ERROR: " + ex.Message);
                                    }
                                }
                            }
                            else
                            {
                                Console.WriteLine("Need to start bus before start TBL");
                            }
                            break;
                        case "powoff":
                            if (tbl.OP_MODE != ControlOpMode.NoInit)
                            {
                                try
                                {
                                    tbl.PowerOff();
                                    Console.WriteLine("TBL is stopped successfully");
                                }
                                catch (Exception ex)
                                {
                                    Console.WriteLine("TBL could not stop");
                                    Console.WriteLine("TBL ERROR: " + ex.Message);
                                }
                            }
                            else
                            {
                                Console.WriteLine("TBL is already stopped,please start firstly");
                            }
                            break;
                        case "autocenter":
                            tbl.AUTOCENTER_SW = true;
                            Thread.Sleep(500);
                            Console.Write("Moving.");
                            while ((wsd.OP_MODE != ControlOpMode.Idle) || (otcxyz.OP_MODE != ControlOpMode.Idle) || (tbl.OP_MODE != ControlOpMode.Idle) || (otcxyz.AB_OPMODE != ControlOpMode.Idle))
                            {
                                Console.Write(" .");
                                Thread.Sleep(500);
                                Console.Write(" .");
                                Thread.Sleep(500);
                                Console.Write(" .");
                            }
                            Console.WriteLine("");
                            Console.WriteLine("Auto Center Finished");
                            tbl.AUTOCENTER_SW = false;
                            break;
                        case "getstat":
                            Console.WriteLine("TBL Status: " + tbl.OP_MODE.ToString());
                            break;
                        case "getpos":
                            Console.WriteLine("TBL Position: " + "X-" + tbl.X_POS.ToString() + " Y-" + tbl.Y_POS.ToString() + " Z-" + tbl.Z_POS.ToString());
                            break;
                        case "move":
                            if (cmdArray.Length < 3)
                            {
                                Console.WriteLine("The command tbl move will move table to designated position");
                                Console.WriteLine("tbl move [-x xpos] [-y ypos] [-z zpos]");
                                break;
                            }
                             var options = new TBLOption();
                            var parser = new CommandLine.Parser(with => with.HelpWriter = Console.Out);
                             if (parser.ParseArgumentsStrict(SplitArray(cmdArray,2,cmdArray.Length-1), options, () => Console.WriteLine()))
                              {
                                  try
                                  {
                                      tbl.FloatBtnDown();
                                      if (options.Xpos != -8192.00)
                                      {
                                          tbl.X_POS = options.Xpos;
                                      }
                                      if (options.Ypos != -8192.00)
                                      {
                                          tbl.Y_POS = options.Ypos;
                                      }
                                      tbl.FloatBtnUp();
                                      if (options.ZPos != -8192.00)
                                      {
                                          if (tbl.Z_POS < options.ZPos)
                                          {
                                              tbl.ZUpBtnDown();
                                              tbl.Z_POS = options.ZPos;
                                              tbl.ZUpBtnUp();
                                          }
                                          else
                                          {
                                              tbl.ZDownBtnDown();
                                              tbl.Z_POS = options.ZPos;
                                              tbl.ZDownBtnUp();
                                          }
                                      }
                                      Thread.Sleep(500);
                                      Console.WriteLine("TBL Move Finished!");
                                  }
                                  catch (Exception ex)
                                  {
                                      Console.WriteLine("TBL Move Error: " + ex.Message);
                                  }
                              }
                            break;
                        default:
                            Console.WriteLine("unknown TBL command,please make sure the command format is correct!");
                            break;
                    }
                    break;
                case "wsdbk":
                    switch (cmdArray[1])
                    { 
                        case "eject":
                            try
                            {
                                wsd.EjectDetector();
                                Console.WriteLine("Detector ejected");
                            }
                            catch (Exception ex)
                            {
                                Console.WriteLine("Eject Error: " + ex.Message);
                            }
                            Thread.Sleep(1000);
                            break;
                        case "insert":
                            try
                            {
                                wsd.InsertDetector(Byte.Parse(cmdArray[2]));
                                Console.WriteLine("Detector " + wsd.DET_ID.ToString() + " is inserted in Wall Bucky successfully");
                            }
                            catch (Exception ex)
                            {
                                Console.WriteLine("detector inserting failed");
                                Console.WriteLine("Error: " + ex.Message);
                            }
                            break;
                        case "check":
                            Console.WriteLine("WSD Detector: " + wsd.DET_ID.ToString());
                            break;
                        default:
                            Console.WriteLine("unknown WSD bucky command,please make sure the command format is correct!");
                            break;
                    }
                    break;
                case "tblbk":
                    switch (cmdArray[1])
                    {
                        case "eject":
                            try
                            {
                                tbl.EjectDetector();
                                Console.WriteLine("Detector ejected");
                            }
                            catch (Exception ex)
                            {
                                Console.WriteLine("Eject Error: " + ex.Message);
                            }
                            Thread.Sleep(1000);
                            break;
                        case "insert":
                            try
                            {
                                tbl.InsertDetector(Byte.Parse(cmdArray[2]));
                                Console.WriteLine("Detector " + tbl.DET_ID.ToString() + " is inserted in Table Bucky successfully");
                            }
                            catch (Exception ex)
                            {
                                Console.WriteLine("detector inserting failed");
                                Console.WriteLine("Error: " + ex.Message);
                            }
                            break;
                        case "check":
                            Console.WriteLine("TBL Detector: " + wsd.DET_ID.ToString());
                            break;
                        default:
                            Console.WriteLine("unknown TBL bucky command,please make sure the command format is correct!");
                            break;
                    }
                    break;

                #region System CMD
                case "system":
                    switch (cmdArray[1])
                    {
                        case "up":
                            if (!busStarted)
                            {
                                try
                                {
                                    bus.StartBus();
                                    busStarted = true;
                                    Console.WriteLine("Can bus started successfully!");
                                }
                                catch (Exception ex)
                                {
                                    Console.WriteLine("Can bus could not start!");
                                    Console.WriteLine("Error: " + ex.Message);
                                    break;
                                }
                            }
                            else
                            {
                                Console.WriteLine("Can bus is already running,please run system power up when state is poweroff");
                            }
                            if (otcxyz.OP_MODE == ControlOpMode.NoInit)
                            {
                                try
                                {
                                    otcxyz.PowerUp();
                                    Console.WriteLine("OTC started successfully!");
                                }
                                catch (Exception ex)
                                {
                                    Console.WriteLine("OTC could not start!");
                                    Console.WriteLine("Error: " + ex.Message);
                                    break;
                                }

                            }
                            else
                            {
                                Console.WriteLine("OTC is already running");
                            }
                            if (wsd.OP_MODE == ControlOpMode.NoInit)
                            {
                                try
                                {
                                    wsd.PowerUp();
                                    Console.WriteLine("WSD started successfully!");
                                }
                                catch (Exception ex)
                                {
                                    Console.WriteLine("WSD could not start!");
                                    Console.WriteLine("Error: " + ex.Message);
                                    break;
                                }

                            }
                            else
                            {
                                Console.WriteLine("WSD is already running");
                            }
                            if (tbl.OP_MODE == ControlOpMode.NoInit)
                            {
                                try
                                {
                                    tbl.PowerUp();
                                    Console.WriteLine("TBL started successfully!");
                                }
                                catch (Exception ex)
                                {
                                    Console.WriteLine("TBL could not start!");
                                    Console.WriteLine("Error: " + ex.Message);
                                    break;
                                }

                            }
                            else
                            {
                                Console.WriteLine("TBL is already running");
                            }
                            if (!col.POW_ON)
                            {
                                try
                                {
                                    col.PowerUp();
                                    Console.WriteLine("Collimator started successfully!");
                                }
                                catch (Exception ex)
                                {
                                    Console.WriteLine("Collimator could not start!");
                                    Console.WriteLine("Error: " + ex.Message);
                                    break;
                                }
                            }
                            else
                            {
                                Console.WriteLine("Collimator is already running");
                            }

                            if (busStarted && col.POW_ON && (otcxyz.OP_MODE == ControlOpMode.NoInit) && (wsd.OP_MODE == ControlOpMode.NoInit) && (otcxyz.AB_OPMODE == ControlOpMode.NoInit) && (tbl.OP_MODE == ControlOpMode.NoInit))
                            {
                                Console.WriteLine("Virtual Lab System Started Successfully!");
                            }
                            break;

                        case "off":
                            if (otcxyz.OP_MODE != ControlOpMode.NoInit)
                            {
                                try
                                {
                                    otcxyz.PowerOff();
                                    Console.WriteLine("OTC stopped successfully!");
                                }
                                catch (Exception ex)
                                {
                                    Console.WriteLine("OTC could not stop!");
                                    Console.WriteLine("Error: " + ex.Message);
                                    break;
                                }

                            }
                            else
                            {
                                Console.WriteLine("OTC is already stopped");
                            }
                            if (wsd.OP_MODE != ControlOpMode.NoInit)
                            {
                                try
                                {
                                    wsd.PowerOff();
                                    Console.WriteLine("WSD stopped successfully!");
                                }
                                catch (Exception ex)
                                {
                                    Console.WriteLine("WSD could not stop!");
                                    Console.WriteLine("Error: " + ex.Message);
                                    break;
                                }

                            }
                            else
                            {
                                Console.WriteLine("WSD is already stopped");
                            }
                            if (tbl.OP_MODE != ControlOpMode.NoInit)
                            {
                                try
                                {
                                    tbl.PowerOff();
                                    Console.WriteLine("TBL stopped successfully!");
                                }
                                catch (Exception ex)
                                {
                                    Console.WriteLine("TBL could not stop!");
                                    Console.WriteLine("Error: " + ex.Message);
                                    break;
                                }

                            }
                            else
                            {
                                Console.WriteLine("TBL is already stopped");
                            }
                            if (col.POW_ON)
                            {
                                try
                                {
                                    col.PowerOff();
                                    Console.WriteLine("Collimator stopped successfully!");
                                }
                                catch (Exception ex)
                                {
                                    Console.WriteLine("Collimator could not stop!");
                                    Console.WriteLine("Error: " + ex.Message);
                                    break;
                                }
                            }
                            else
                            {
                                Console.WriteLine("Collimator is already stopped");
                            }
                            if (busStarted)
                            {
                                try
                                {
                                    bus.StopBus();
                                    busStarted = false;
                                    Console.WriteLine("Can bus stopped successfully!");
                                }
                                catch (Exception ex)
                                {
                                    Console.WriteLine("Can bus could not stop!");
                                    Console.WriteLine("Error: " + ex.Message);
                                    break;
                                }
                            }
                            else
                            {
                                Console.WriteLine("Can bus is already stopped,please run system power off when system is running");
                            }
                            break;
                        case "getstat":
                            Console.WriteLine("Can Bus: " + busStarted.ToString());
                            Console.WriteLine("OTC XYZ: " + otcxyz.OP_MODE.ToString());
                            Console.WriteLine("OTC AB: " + otcxyz.AB_OPMODE.ToString());
                            Console.WriteLine("WSD: " + wsd.OP_MODE.ToString());
                            Console.WriteLine("TBL: " + tbl.OP_MODE.ToString());
                            Console.WriteLine("COLLIMATOR:" + col.POW_ON.ToString());
                            break;
                        default:
                            Console.WriteLine("unknown SYS command,please make sure the command format is correct!");
                            break;
                    }
                    break;

                #endregion

               

                default:
                    Console.WriteLine("unknown SYS command,please make sure the command format is correct!");
                    break;
            }

        }
    }
}
