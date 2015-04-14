using System;
using System.Collections.Generic;
using System.Text;
using System.IO.Ports;
using System.Threading;

namespace PCUSimulationNS
{
    class SerialClass
    {
        private SerialPort serialport = null;
        private string port;
        private const int ACKTimeOut = 1200;

        private const byte STX = 0x02;
        private const byte EXT = 0x03;
        private const byte ACK = 0x06;
        private const byte NAK = 0x15;

        private const int checksumLen = 4;
        private const int packetsizeLen = 4;

        private AutoResetEvent waitForConsoleACK = new AutoResetEvent(false);
        private AutoResetEvent waitForConsoleNAK = new AutoResetEvent(false);

        public string PORT
        {
            get { return port; }
            set { port = value; }
        }

        public int READ_TIMEOUT
        {
            get { return serialport.ReadTimeout; }
            set { serialport.ReadTimeout = value; }
        }

        #region Constructor
        /// <summary>
        /// Default Constructor
        /// </summary>
        public SerialClass()
        {
            serialport = new SerialPort("COM17");
            if (serialport == null)
            {
                throw new Exception("COM17 failed to initailize,please check if the port exists.");
            }
            port = "COM17";
        }

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="portNumber">The Serial Port Number</param>
        public SerialClass(string portNumber)
        {
            serialport = new SerialPort(portNumber);
            if (serialport == null)
            {
                throw new Exception(portNumber + " failed to initailize,please check if the port exists.");
            }
            port = portNumber;
        }

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="portNumber">The Serial Port Number</param>
        public SerialClass(string portNumber,int baudRate,int dataBit,StopBits stopBits,Parity parity)
        {
            serialport = new SerialPort(portNumber);
            serialport.BaudRate = baudRate;
            serialport.DataBits = dataBit;
            serialport.StopBits = stopBits;
            serialport.Parity = parity;
            if (serialport == null)
            {
                throw new Exception(portNumber + " failed to initailize,please check if the port exists.");
            }
            port = portNumber;
        }

        #endregion

        /// <summary>
        /// Open the serial port
        /// </summary>
        public void OpenPort()
        {
            if (serialport != null && !serialport.IsOpen)
            {
                try
                {
                    serialport.Open();
                }
                catch (System.Exception e)
                {
                    throw new Exception("Exception in opening: "+e.Message);
                }
                if (!serialport.IsOpen)
                {
                    throw new Exception("Failed to open serial port!");
                }
            }
            else
            {
                if (serialport == null)
                {
                    throw new Exception("Open:Serial port is not initialized");
                }
            }
        }

        /// <summary>
        /// Write function
        /// </summary>
        /// <param name="command">the command datas</param>
        public void Write(byte[] command)
        {
            //Console.Write("Send: ");
            //foreach (byte a in command)
            //{
            //    Console.Write(((char)a).ToString());
            //}
           // Console.WriteLine("");
            if (serialport != null)
            {
                if (serialport.IsOpen)
                {
                    try
                    {
                        serialport.Write(command, 0, command.Length);
                    }
                    catch (System.Exception e)
                    {
                        throw new Exception("Failed to write datas to serial port: " + e.Message);
                    }
                }
                else
                {
                    throw new Exception("Write:Serial port is not opened");
                }
            }
            else
            {
                throw new Exception("Write:Serial port is not initialized");
            }
        }

        /// <summary>
        /// Read function
        /// </summary>
        /// <param name="input">input buffer</param>
        /// <param name="offset">reading offset</param>
        /// <param name="count">reading count </param>
        /// <returns></returns>
        public int Read(ref byte[] input, int offset, int count)
        {
            int value = 0;
            if (serialport != null)
            {
                if (serialport.IsOpen)
                {
                    try
                    {
                        value = serialport.Read(input, offset, count);
                        
                    }
                    catch (System.Exception e)
                    {
                        throw new Exception("Failed to read datas from serial port: " + e.Message);
                    }
                }
                else
                {
                    throw new Exception("Read:Serial port is not opened");
                }
            }
            else
            {
                throw new Exception("Read:Serial port is not initialized");
            }
            return value;
        }

        /// <summary>
        /// Read I/O board reply until get whole frame
        /// </summary>
        /// <param name="input">intput buffer</param>
        /// <param name="count">reading count</param>
        /// <returns></returns>
        public int ReadReply(ref byte[] input, int count)
        {
            int value = 0;
            int countLeft = count;
            int readCount = 0;
            byte[] tempBuffer = new byte[count];
            if (serialport != null)
            {
                if (serialport.IsOpen)
                {
                    while (countLeft > 0)
                    {
                        try
                        {
                            value = serialport.Read(tempBuffer, 0, countLeft);
                            if (value < 0)
                            {
                                break;
                            }
                            if (value == 0)
                            {
                                throw new TimeoutException("read timeout");
                            }
                            else
                            {
                                for (int i = 0; i < value; i++)
                                {
                                    input[count - countLeft + i] = tempBuffer[i];
                                }
                                countLeft = countLeft - value;
                                readCount += value;
                            }
                        }
                        catch (System.Exception e)
                        {
                            throw new Exception("Failed to read datas from serial port: " + e.Message);
                        }
                    }
                }
                else
                {
                    throw new Exception("Read:Serial port is not opened");
                }
            }
            else
            {
                throw new Exception("Read:Serial port is not initialized");
            }
            return readCount;
        }


        /// <summary>
        /// unpack the command string from console
        /// </summary>
        /// <param name="commandStr"></param>
        /// <returns></returns>
        private string UnpackCommand(string commandStr)
        {
            Byte[] ack = new Byte[1];
            ack[0] = ACK;
            Write(ack);
            string checksumStrReceived = commandStr.Substring(0, 4);
            UInt16 checksum = CalculateCheckSum(commandStr.Substring(4, commandStr.Length - 4));

            string checksumStr = checksum.ToString("X");
            checksumStr = string.Format("{0,4}", checksumStr);
            checksumStr = checksumStr.Replace(' ', '0');

            if (checksumStr == checksumStrReceived)
            {
                return commandStr.Substring(8, commandStr.Length - 8);
            }
            else
            {
                throw new Exception("received UPS command from console broken");
            }   
        }


        /// <summary>
        /// Read a console command 
        /// </summary>
        /// <returns>the command string</returns>
        public string ReadCommand()
        {
            string command = "";
            int byteRead;
            bool needStart = true;
            if (serialport != null)
            {
                if (serialport.IsOpen)
                {
                    while (true)
                    {
                        try
                        {
                            byteRead = (byte)serialport.ReadByte();
                            if (byteRead == -1)
                            {
                                continue;
                            }
                            if (needStart)
                            {
                                if (byteRead == STX)
                                {
                                    needStart = false;
                                }
                                else
                                {
                                    if (byteRead == ACK)
                                    {
                                        waitForConsoleACK.Set();
                                    }
                                    continue;
                                }
                            }
                            else
                            {
                                if (byteRead != EXT)
                                {
                                    command = command.Insert(command.Length, ((char)byteRead).ToString());
                                }
                                else
                                {
                                    Console.WriteLine("received: "+command);
                                    return UnpackCommand(command);
                                }
                            }
                        }
                        catch (System.Exception e)
                        {
                            throw new Exception("Failed to read datas from console serial port: " + e.Message);
                        }
                    }
                }
                else
                {
                    throw new Exception("Read:console serial port is not opened");
                }
            }
            else
            {
                throw new Exception("Read:console serial port is not initialized");
            }
        }


        /// <summary>
        /// Send PCU Command to Console
        /// </summary>
        /// <param name="command"></param>
        public bool SendPCUResponse(string command)
        {
            int retryCount = 3;
            string sendCommand = GeneratePCUMessage(command);
            int bufferSize = sendCommand.Length + 2;
            Byte[] responseBuffer = new Byte[bufferSize];
            responseBuffer[0] = STX;
            responseBuffer[bufferSize - 1] = EXT;
            for (int i = 0; i < sendCommand.Length; i++)
            {
                responseBuffer[i + 1] = (Byte)sendCommand[i];
            }
            while (retryCount > 0)
            {
                try
                {
                    Write(responseBuffer);
                    if (waitForConsoleACK.WaitOne(200))
                    {
                        Console.WriteLine("Get ACK from Console");
                        return true;
                    }
                    else
                    {
                        Console.WriteLine("Retry "+retryCount.ToString());
                        retryCount--;
                    }
                }
                catch (Exception e)
                {
                    throw new Exception(e.Message);
                }
            }
            return false;
        }

        /// <summary>
        /// Calculate the checksum of console-Pcu command
        /// </summary>
        /// <param name="strCmd"></param>
        /// <returns></returns>
        private  UInt16 CalculateCheckSum(string strCmd)
        {
            UInt16 result = 0;
            for (int i = 0; i < strCmd.Length; i++)
            {
                result += (UInt16)(strCmd[i]);
            }
            return result;
        }

        /// <summary>
        /// generate PCU message with checksum and packet length
        /// </summary>
        /// <param name="cmd"></param>
        /// <returns></returns>
        private string GeneratePCUMessage(string cmd)
        {
            string cmdString = "";
            UInt16 checksum = 0;
            UInt16 packetSize = 0;
            string checksumStr;
            string packetSizeStr;

            packetSize = (UInt16)(1 + checksumLen + packetsizeLen + cmd.Length + 1);
            packetSizeStr = packetSize.ToString("X");
            packetSizeStr = string.Format("{0,4}", packetSizeStr);
            packetSizeStr = packetSizeStr.Replace(' ', '0');

            checksum = CalculateCheckSum(packetSizeStr + cmd);
            checksumStr = checksum.ToString("X");
            checksumStr = string.Format("{0,4}", checksumStr);
            checksumStr = checksumStr.Replace(' ', '0');

            cmdString = string.Format("{0}{1}{2}", checksumStr, packetSizeStr, cmd);
            return cmdString;
        }

        /// <summary>
        /// Clear the serial receiving buffer
        /// </summary>
        public void ClearReadBuffer()
        {
            if (serialport != null)
            {
                try
                {
                    serialport.DiscardInBuffer();
                }
                catch (Exception )
                {
                    throw new Exception("Failed to clear receiving buffer on" + serialport.PortName);
                }
            }
        }

        /// <summary>
        /// Close function
        /// </summary>
        public void Close()
        {
            if (serialport != null)
            {
                if (serialport.IsOpen)
                {
                    try
                    {
                        serialport.Close();
                        serialport.Dispose();
                        serialport = null;
                    }
                    catch (System.Exception e)
                    {
                        serialport = null;
                        throw new Exception("Failed to close the serial port: " + e.Message);
                    }
                }
                else
                {
                        serialport.Dispose();
                        serialport = null;
                }
            }
        }
        

    }
}
