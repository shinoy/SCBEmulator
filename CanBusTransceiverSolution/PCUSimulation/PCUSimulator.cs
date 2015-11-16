using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace PCUSimulationNS
{
    public enum Status
    {
        ON=0,
        OFF
    }

    public delegate void BuckyStartEventHandler();

    public class PCUSimulator : MarshalByRefObject
    {
        private static SerialClass ioport = new SerialClass("COM18");
        private static SerialClass consoleport = new SerialClass("COM19", 19200, 8, System.IO.Ports.StopBits.One, System.IO.Ports.Parity.None);
        private bool isConnected = false;
        private bool isAutoExpMode = false;

        private static readonly object cmdLockObj = new object();

        private Thread handSwitchCheckThread = null;
        private Thread pcuCommunicationThread = null;
        private Thread handleConsoleCmdThread = null;

        public static event BuckyStartEventHandler BuckyStartEvent;

        Byte[] comBuffer = new Byte[9];
        Byte[] checkCommand = new Byte[9];
        Byte[] receiveBuffer = new Byte[9];

        #region Command Definition

        const byte R_BIT_ON = 0x1;
        const byte R_BIT_OFF = 0x2;
        const byte R_ALL_ON = 0x3;
        const byte R_ALL_OFF = 0x4;
        const byte R_GROUP_ON = 0x5;
        const byte R_GROUP_OFF = 0x6;
        const byte READ_STATUS = 0x7;
        const byte R_OUT = 0x8;

        const byte R_DELAY_OFF_A = 0x12;
        const byte R_DELAY_OFF_B = 0x13;
        const byte R_DELAY_OFF_C = 0x14;

        const byte GET_ADDRESS = 0x10;
        const byte SET_ADDRESS = 0X11;

        const byte HEADER0 = 0x0;
        const byte HEADER1 = 0x5A;
        const byte DEVICE_CLASS = 0x53;
        const byte addressVar = 0x00;

        const int FRAMESIZE = 9;

        #endregion

        #region constructor
        public PCUSimulator()
        {
            comBuffer[0] = checkCommand[0] = HEADER0;
            comBuffer[1] = checkCommand[1] = HEADER1;
            comBuffer[2] = checkCommand[2] = DEVICE_CLASS;
            comBuffer[3] = checkCommand[3] = addressVar;
            checkCommand[4] = 0x07;
            checkCommand[8] = GetCheckSum2();

            preInStatus = Status.OFF;
            expInStatus = Status.OFF;
        }

        //public PCUSimulator(string controlBoardPort,string consolePort)
        //{
        //    ioport = new SerialClass(controlBoardPort);
        //    consoleport = new SerialClass(consolePort, 19200, 8, System.IO.Ports.StopBits.One, System.IO.Ports.Parity.None);
        //    comBuffer[0] = checkCommand[0] = HEADER0;
        //    comBuffer[1] = checkCommand[1] = HEADER1;
        //    comBuffer[2] = checkCommand[2] = DEVICE_CLASS;
        //    comBuffer[3] = checkCommand[3] = addressVar;
        //    checkCommand[4] = 0x07;
        //    checkCommand[8] = GetCheckSum2();

        //    preInStatus = Status.OFF;
        //    expInStatus = Status.OFF;
        //}
        #endregion

        #region SW Defination

        private Status preOutStatus = Status.OFF;
        private Status expOutStatus = Status.OFF;
        private Status buckyReadyStatus = Status.OFF;
        private Status preInStatus ;
        private Status expInStatus ;
        private Status buckyStartStatus = Status.OFF;

        #endregion 

        /// <summary>
        /// Connect to control board
        /// </summary>
        public void Connect()
        {
            ioport.READ_TIMEOUT = 50;
            ioport.OpenPort();
            consoleport.OpenPort();
            this.isConnected = true;
            handSwitchCheckThread = new Thread(new ThreadStart(CheckHandSwitch));
            handSwitchCheckThread.IsBackground = true;
            handSwitchCheckThread.Priority = ThreadPriority.AboveNormal;
            handSwitchCheckThread.Start();
           
            pcuCommunicationThread = new Thread(new ThreadStart(ConsoleUPSCommandHandle));
            pcuCommunicationThread.IsBackground = true;
            pcuCommunicationThread.Priority = ThreadPriority.AboveNormal;
            pcuCommunicationThread.Start();
        }

        /// <summary>
        /// Disconnected from control board
        /// </summary>
        public void Disconnect()
        {
            handSwitchCheckThread.Abort();
            pcuCommunicationThread.Abort();
            handSwitchCheckThread = null;
            pcuCommunicationThread = null;
            ioport.Close();
            consoleport.Close();
            ioport = null;
            consoleport = null;
            this.isConnected = false;
        }

        /// <summary>
        /// Caculate the checksum
        /// </summary>
        /// <returns></returns>
        private byte GetCheckSum()
        {
            byte sum = 0;
            for (int i = 0; i < 8; i++)
            {
                sum += comBuffer[i];
            }
            return (byte)sum;
        }


        public void TestRomote()
        {
           
           // MessageBox.Show("remote channel ok");
        }

        /// <summary>
        /// Caculate the checkcommand checksum
        /// </summary>
        /// <returns></returns>
        private byte GetCheckSum2()
        {
            byte sum = 0;
            for (int i = 0; i < 8; i++)
            {
                sum += checkCommand[i];
            }
            return (byte)sum;
        }

        /// <summary>
        /// Clear the serial buffer
        /// </summary>
        private void ClearBuffer()
        {
            comBuffer[4] = 0x00;
            comBuffer[5] = 0x00;
            comBuffer[6] = 0x00;
            comBuffer[7] = 0x00;
            comBuffer[8] = 0x00;
        }

      
        /// <summary>
        /// Process the UPS command from console
        /// </summary>
        /// <param name="command"></param>
        private void ProcessUPSCommand(object command)
        {
            lock (cmdLockObj)
            {
                switch ((string)command)
                {
                    case "EHO":
                        bool result = consoleport.SendPCUResponse("EHO Y");
                        Console.WriteLine("EHO True");

                        break;
                    case "HVR":
                        consoleport.SendPCUResponse("HVR Y1.0");
                        break;
                    case "SVR":
                        consoleport.SendPCUResponse("SVR Y2.0");
                        break;
                    case "PWD":
                        consoleport.SendPCUResponse("PWD Y");
                        break;
                    case "RAY":
                        consoleport.SendPCUResponse("RAY Y");
                        BUCKY_READY = Status.ON;
                        Thread.Sleep(100);
                        BUCKY_READY = Status.OFF;
                        break;
                    case "UPS":

                        break;
                    case "UPA":

                        break;
                    case "UPB":

                        break;
                    default:
                       // throw new Exception("Unknow UPS Command Received");
                        break;
                }
            }
        }

        /// <summary>
        /// check the handswitch status
        /// </summary>
        private void CheckHandSwitch()
        {
            int readCount = 0;
            ioport.ClearReadBuffer();
            while(isConnected)
            {
                if (isAutoExpMode == true)
                {
                    continue;
                }
             //   Console.WriteLine("checking "+isConnected.ToString());
                try
                {
                    ioport.Write(checkCommand);
                   // Console.WriteLine("Write command to UPS");
                    if ((readCount = ioport.ReadReply(ref receiveBuffer, FRAMESIZE)) != FRAMESIZE)
                    {
                        Console.WriteLine("Checking input status error,don't receive correct frame length");
                      //  throw new Exception("Checking input status error,don't receive correct frame length");
                    }
                  //  Console.WriteLine("Read reply from UPS");
                    if ((receiveBuffer[0] == HEADER0) && (receiveBuffer[1] == HEADER1))
                    {
                        if ((receiveBuffer[6] & 0x01) != 0)
                        {
                            preInStatus = Status.OFF;
                            if (PRE_OUT == Status.ON)
                            {
                                PRE_OUT = Status.OFF;
                            }
                        }
                        else
                        {
                            preInStatus = Status.ON;
                            if (PRE_OUT == Status.OFF)
                            {
                                PRE_OUT = Status.ON;
                            }


                        }
                     //   Console.WriteLine("Check pre");
                        if ((receiveBuffer[6] & 0x02) != 0)
                        {
                            expInStatus = Status.OFF;
                            if (EXP_OUT == Status.ON)
                            {
                                EXP_OUT = Status.OFF;
                            }
                        }
                        else
                        {
                            expInStatus = Status.ON;
                            if (EXP_OUT == Status.OFF)
                            {
                                EXP_OUT = Status.ON;
                            }
                        }
                       // Console.WriteLine("Check Exp");
                        if ((receiveBuffer[6] & 0x04) != 0)
                        {
                            buckyStartStatus = Status.OFF;
                        }
                        else
                        {
                            if (buckyStartStatus == Status.OFF)
                            {
                                if (BuckyStartEvent != null)
                                {
                                   // Console.WriteLine("bucky start...");
                                    BuckyStartEvent();
                                }

                                buckyStartStatus = Status.ON;
                            }

                        }
                       // Console.WriteLine("Check bucky start");
                    }
                    else
                    {
                        Console.WriteLine("Checking input status error,don't receive correct frame format");
                      //  throw new Exception("Checking input status error,don't receive correct frame format");
                    }
                }
                catch (TimeoutException e)
                {
                    Console.WriteLine("timeout occurs once: "+e.Message);
                }
                catch (Exception e)
                {
                   Console.WriteLine("error while checking handswitch " + e.Message);
                  //throw new Exception("Checking input status error as" + e.Message);
                }
            }
        }

        /// <summary>
        /// communicate with console to simulator UPS
        /// </summary>
        private void ConsoleUPSCommandHandle()
        {
            string commandReceived;
         //   consoleport.ClearReadBuffer();
            while (isConnected)
            {
                try
                {
                    commandReceived = consoleport.ReadCommand();
                    handleConsoleCmdThread = new Thread(new ParameterizedThreadStart(ProcessUPSCommand));
                    handleConsoleCmdThread.Priority = ThreadPriority.Normal;
                    handleConsoleCmdThread.Start(commandReceived);
                }
                catch (Exception e)
                {
                    MessageBox.Show(e.Message);
                   // throw new Exception("UPS command received error: "+e.Message);
                }
            }
        }

        #region SW Property
        public Status PRE_OUT
        {
            get { return preOutStatus; }
            set
            {
                if (value == Status.ON)
                {
                    this.ClearBuffer();
                    comBuffer[4] = 0x01;
                    comBuffer[5] = 0x03;
                    comBuffer[8] = this.GetCheckSum();
                }
                else
                {
                    this.ClearBuffer();
                    comBuffer[4] = 0x02;
                    comBuffer[5] = 0x03;
                    comBuffer[8] = this.GetCheckSum();
                }
                try
                {
                    ioport.Write(comBuffer);
                    preOutStatus = value;
                }
                catch (Exception e)
                {
                    throw new Exception("Prepare output " + value.ToString() + ":" + e.Message);
                }
            }
        }

        public Status EXP_OUT
        {
            get { return expOutStatus; }
            set
            {
                if (value == Status.ON)
                {
                    this.ClearBuffer();
                    comBuffer[4] = 0x01;
                    comBuffer[5] = 0x02;
                    comBuffer[8] = this.GetCheckSum();
                }
                else
                {
                    this.ClearBuffer();
                    comBuffer[4] = 0x02;
                    comBuffer[5] = 0x02;
                    comBuffer[8] = this.GetCheckSum();
                }

                try
                {
                    ioport.Write(comBuffer);
                    expOutStatus = value;
                }
                catch (Exception e)
                {
                    throw new Exception("Expose output " + value.ToString() + ":" + e.Message);
                }
            }
        }


        public Status BUCKY_READY
        {
            get { return buckyReadyStatus; }
            set
            {
                if (value == Status.ON)
                {
                    this.ClearBuffer();
                    comBuffer[4] = 0x01;
                    comBuffer[5] = 0x01;
                    comBuffer[8] = this.GetCheckSum();
                }
                else
                {
                    this.ClearBuffer();
                    comBuffer[4] = 0x02;
                    comBuffer[5] = 0x01;
                    comBuffer[8] = this.GetCheckSum();
                }

                try
                {
                    ioport.Write(comBuffer);
                    buckyReadyStatus = value;
                }
                catch (Exception e)
                {
                    throw new Exception("BuckyReady output " + value.ToString() + ":" + e.Message);
                }
            }
        }

        public void EnAutoExp()
        {
            BUCKY_READY = Status.OFF;
            PRE_OUT = Status.OFF;
            EXP_OUT = Status.OFF;
            isAutoExpMode = true;
        }

        public void DisAutoExp()
        {
            BUCKY_READY = Status.OFF;
            PRE_OUT = Status.OFF;
            EXP_OUT = Status.OFF;
            isAutoExpMode = false;
        }

        public void Pre()
        {
            PRE_OUT = Status.ON;
        }

        public void Release()
        {
            PRE_OUT = Status.OFF;
            EXP_OUT = Status.OFF;
            buckyStartStatus = Status.OFF;
        }
   

        public void Expose()
        {
           
                EXP_OUT = Status.ON;
                if (BuckyStartEvent != null)
                {
                    // Console.WriteLine("bucky start...");
                    BuckyStartEvent();
                }

                buckyStartStatus = Status.ON;
           
        }


        public void RegExp(int preTime, int expTime)
        {
      
                PRE_OUT = Status.ON;
                Thread.Sleep(preTime);
                EXP_OUT = Status.ON;
                Thread.Sleep(expTime);
                EXP_OUT = Status.OFF;
                PRE_OUT = Status.OFF;
      
        }
        #endregion
    }

}
