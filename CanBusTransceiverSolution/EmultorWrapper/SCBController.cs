using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SCBProtocalWrapper;
using PositionerSimulationNS;
using PCUSimulationNS;
using System.Threading;
using CanBusChannelNS;
using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting.Channels.Tcp;

namespace EmultorWrapper
{
    public struct OTCButton
    {
        public bool XRelBtn;
        public bool YRelBtn;
        public bool ZRelBtn;
        public bool XYZRelBtn;
        public bool ARelBtn;
        public bool BRelBtn;
    }

    public struct WSDButton
    {
        public bool ZRelBtn;
        public bool AutoCenterBtn;
        public bool AutoPosBtn;
    }

    public struct TBLButton
    {
        public bool FloatRelBtn;
        public bool AutoCenterBtn;
    }

    public class SCBController
    {
        #region private members
        private bool isPowerOn = false;

        // controllers and CAN bus 
        private CanBusChannelCls bus = new CanBusChannelCls();

        private OTCXYZControllerCLS otcxyz = null;

        private WSDControllerCLS wsd = null;

        private TBLControllerCLS tbl = null;

        private CollimatorCLS col = null;

        private PCUSimulator pcu = null;

        /****************   Buttons status **************/
        // OTC buttons
        private OTCButton otcBtn = new OTCButton();

        // WSD buttons
        private WSDButton wsdBtn = new WSDButton();

        // TBL buttons
        private TBLButton tblBtn = new TBLButton();

        #endregion



        #region ctor

        public SCBController()
        {
            otcBtn.ARelBtn = otcBtn.BRelBtn = otcBtn.XRelBtn = otcBtn.XYZRelBtn = otcBtn.YRelBtn = otcBtn.ZRelBtn = false;
            wsdBtn.ZRelBtn = tblBtn.FloatRelBtn = false;
            UnregisterPCUService();
            Thread.Sleep(500);
            RegisterPCUService();
            pcu = getPCUObject();
            pcu.Connect();
            bus.Connect();
            otcxyz = new OTCXYZControllerCLS(bus);
            wsd = new WSDControllerCLS(bus);
            tbl = new TBLControllerCLS(bus);
            col = new CollimatorCLS(bus);

        }

        #endregion

        #region private methods

        private void BuckyStartHandler()
        {
            //   MessageBox.Show("Bucky Start Event");
            tbl.BuckyStartEventHandler();
        }

        private void UnregisterPCUService()
        {

            try
            {
                IChannel[] channels = ChannelServices.RegisteredChannels;


                foreach (IChannel eachChannel in channels)
                {
                    if (eachChannel.ChannelName == "PCUChannel")
                    {
                        TcpChannel tcpChannel = (TcpChannel)eachChannel;


                        tcpChannel.StopListening(null);

                        ChannelServices.UnregisterChannel(tcpChannel);
                    }
                }
            }
            catch (Exception)
            {

            }
        }

        private void RegisterPCUService()
        {
            try
            {
                TcpServerChannel serverChannel = new TcpServerChannel("PCUChannel", 6688);
                ChannelServices.RegisterChannel(serverChannel, false);
                RemotingConfiguration.RegisterWellKnownServiceType(typeof(PCUSimulator), "PCUSimulater", WellKnownObjectMode.Singleton);
                PCUSimulator.BuckyStartEvent += new BuckyStartEventHandler(BuckyStartHandler);
            }
            catch (RemotingException)
            {
                
            }

        }

        private PCUSimulator getPCUObject()
        {
            try
            {
                ChannelServices.RegisterChannel(new TcpClientChannel(), false);
                PCUSimulator simulator = (PCUSimulator)Activator.GetObject(typeof(PCUSimulator), "tcp://localhost:6688/PCUSimulater");
                simulator.TestRomote();
                if (simulator != null)
                {

                    //  MessageBox.Show("成功获取remote PCU");
                }
                return simulator;

            }
            catch (Exception ex)
            {
                return null;
            }
        }
        #endregion

        #region public methods

        /// <summary>
        /// Power up all axis controllers and UPS
        /// </summary>
        public void PowerUp()
        {
            UnregisterPCUService();
            Thread.Sleep(500);
            RegisterPCUService();
            pcu = getPCUObject();
            pcu.Connect();
            bus.StartBus();
            otcxyz.PowerUp();
            wsd.PowerUp();
            tbl.PowerUp();
            col.PowerUp();
            pcu.Connect();
            isPowerOn = true;
        }

        /// <summary>
        /// Power off all axis controllers and UPS
        /// </summary>
        public void PowerOff()
        {
            isPowerOn = false;
            pcu.Disconnect();
            UnregisterPCUService();
            otcxyz.PowerOff();
            wsd.PowerOff();
            tbl.PowerOff();
            bus.StopBus();

        }

        /****************   OTC Buttons method **************/

        public void OTCXRelP()
        {
            if (isPowerOn && (otcBtn.XRelBtn == false))
            {
                otcBtn.XRelBtn = true;
                otcxyz.XRelBtnPress();
            }
        }

        public void OTCXRelR()
        {
            if (isPowerOn && (otcBtn.XRelBtn == true))
            {
                otcBtn.XRelBtn = false;
                otcxyz.XRelBtnRelease();
            }
        }

        public void OTCYRelP()
        {
            if (isPowerOn && (otcBtn.YRelBtn == false))
            {
                otcBtn.YRelBtn = true;
                otcxyz.YRelBtnPress();
            }
        }

        public void OTCYRelR()
        {
            if (isPowerOn && (otcBtn.YRelBtn == true))
            {
                otcBtn.YRelBtn = false;
                otcxyz.YRelBtnRelease();
            }
        }

        public void OTCZRelP()
        {
            if (isPowerOn && (otcBtn.ZRelBtn == false))
            {
                otcBtn.ZRelBtn = true;
                otcxyz.ZRelBtnPress();
            }
        }

        public void OTCZRelR()
        {
            if (isPowerOn && (otcBtn.ZRelBtn == true))
            {
                otcBtn.ZRelBtn = false;
                otcxyz.ZRelBtnRelease();
            }
        }

        public void OTCXYZRelP()
        {
            if (isPowerOn && (otcBtn.XYZRelBtn == false))
            {
                otcBtn.XYZRelBtn = true;
                otcxyz.XYZRelBtnPress();
            }
        }

        public void OTCXYZRelR()
        {
            if (isPowerOn && (otcBtn.XYZRelBtn == true))
            {
                otcBtn.XYZRelBtn = false;
                otcxyz.XYZRelBtnRelease();
            }
        }

        public void OTCARelP()
        {
            if (isPowerOn && (otcBtn.ARelBtn == false))
            {
                otcBtn.ARelBtn = true;
                otcxyz.ARelBtnPress();
            }
        }

        public void OTCARelR()
        {
            if (isPowerOn && (otcBtn.ARelBtn == true))
            {
                otcBtn.ARelBtn = false;
                otcxyz.ARelBtnRelease();
            }
        }

        public void OTCBRelP()
        {
            if (isPowerOn && (otcBtn.BRelBtn == false))
            {
                otcBtn.BRelBtn = true;
                otcxyz.BRelBtnPress();
            }
        }

        public void OTCBRelR()
        {
            if (isPowerOn && (otcBtn.BRelBtn == true))
            {
                otcBtn.BRelBtn = false;
                otcxyz.BRelBtnRelease();
            }
        }


        /****************   OTC Buttons method **************/

        public void WSDZRelP()
        {
            if (isPowerOn && (wsdBtn.ZRelBtn == false))
            {
                wsdBtn.ZRelBtn = true;
                wsd.ZRelBtnPress();
            }
        }

        public void WSDZRelR()
        {
            if (isPowerOn && (wsdBtn.ZRelBtn == true))
            {
                wsdBtn.ZRelBtn = false;
                wsd.ZRelBtnRelease();
            }
        }

        /****************   TBL Buttons method **************/
        public void TBLFRelP()
        {
            if (isPowerOn && (tblBtn.FloatRelBtn == false))
            {
                tblBtn.FloatRelBtn = true;
                tbl.FLOAT_SW = true;
            }
        }

        public void TBLFRelR()
        {
            if (isPowerOn && (tblBtn.FloatRelBtn == true))
            {
                tblBtn.FloatRelBtn = false;
                tbl.FLOAT_SW = false;
            }
        }

        /****************   Axis Controller movement method **************/

        public void OTCXMove(int step)
        {
            if (isPowerOn && (otcBtn.XYZRelBtn || otcBtn.XRelBtn))
            {
                if (step >= 0)
                {
                    for (int i = 0; i < step; i++)
                    {
                        otcxyz.XStepUp();
                    }
                }
                else
                {
                    for (int i = 0; i < -step; i++)
                    {
                        otcxyz.XStepDown();
                    }
                }
            }
        }

        public void OTCYMove(int step)
        {
            if (isPowerOn && (otcBtn.XYZRelBtn || otcBtn.YRelBtn))
            {
                if (step >= 0)
                {
                    for (int i = 0; i < step; i++)
                    {
                        otcxyz.YStepUp();
                    }
                }
                else
                {
                    for (int i = 0; i < -step; i++)
                    {
                        otcxyz.YStepDown();
                    }
                }
            }
        }

        public void OTCZMove(int step)
        {
            if (isPowerOn && (otcBtn.XYZRelBtn || otcBtn.ZRelBtn))
            {
                if (step >= 0)
                {
                    for (int i = 0; i < step; i++)
                    {
                        otcxyz.ZStepUp();
                    }
                }
                else
                {
                    for (int i = 0; i < -step; i++)
                    {
                        otcxyz.ZStepDown();
                    }
                }
            }
        }

        public void OTCAMove(int step)
        {
            if (isPowerOn && otcBtn.ARelBtn)
            {
                if (step >= 0)
                {
                    for (int i = 0; i < step; i++)
                    {
                        otcxyz.AStepUp();
                    }
                }
                else
                {
                    for (int i = 0; i < -step; i++)
                    {
                        otcxyz.AStepDown();
                    }
                }
            }
        }

        public void OTCBMove(int step)
        {
            if (isPowerOn && otcBtn.BRelBtn)
            {
                if (step >= 0)
                {
                    for (int i = 0; i < step; i++)
                    {
                        otcxyz.BStepUp();
                    }
                }
                else
                {
                    for (int i = 0; i < -step; i++)
                    {
                        otcxyz.BStepDown();
                    }
                }
            }
        }

        public void WSDZMove(int step)
        {
            if (isPowerOn && wsdBtn.ZRelBtn)
            {
                if (step >= 0)
                {
                    for (int i = 0; i < step; i++)
                    {
                        wsd.ZStepUp();
                    }
                }
                else
                {
                    for (int i = 0; i < -step; i++)
                    {
                        wsd.ZStepDown();
                    }
                }
            }
        }

        public void TBLXMove(int step)
        {
            if (isPowerOn && tblBtn.FloatRelBtn)
            {
                if (step >= 0)
                {
                    for (int i = 0; i < step; i++)
                    {
                        tbl.XStepUp();
                    }
                }
                else
                {
                    for (int i = 0; i < -step; i++)
                    {
                        tbl.XStepDown();
                    }
                }
            }
        }

        public void TBLYMove(int step)
        {
            if (isPowerOn && tblBtn.FloatRelBtn)
            {
                if (step >= 0)
                {
                    for (int i = 0; i < step; i++)
                    {
                        tbl.YStepUp();
                    }
                }
                else
                {
                    for (int i = 0; i < -step; i++)
                    {
                        tbl.YStepDown();
                    }
                }
            }
        }

        /****************   Axis Controller automovement method **************/

        public void WSDAutoCenter()
        {
            if (isPowerOn)
            {
                wsd.keypad.AUTO_CENTER_SW = true;
                Thread.Sleep(500);
                while ((wsd.OP_MODE != ControlOpMode.Idle) || (otcxyz.OP_MODE != ControlOpMode.Idle) || (tbl.OP_MODE != ControlOpMode.Idle) || (otcxyz.AB_OPMODE != ControlOpMode.Idle))
                {
                    Thread.Sleep(500);
                }
                wsd.keypad.AUTO_CENTER_SW = false;
            }
        }

        public void WSDAutoPos()
        {
            if (isPowerOn)
            {
                wsd.keypad.AUTO_POS_SW = true;
                Thread.Sleep(500);
                while ((wsd.OP_MODE != ControlOpMode.Idle) || (otcxyz.OP_MODE != ControlOpMode.Idle) || (tbl.OP_MODE != ControlOpMode.Idle) || (otcxyz.AB_OPMODE != ControlOpMode.Idle))
                {
                    Thread.Sleep(500);
                }
                wsd.keypad.AUTO_POS_SW = false;
            }
        }

        public void TBLAutoCenter()
        {
            if (isPowerOn)
            {
                tbl.AUTOCENTER_SW = true;
                Thread.Sleep(500);
                while ((wsd.OP_MODE != ControlOpMode.Idle) || (otcxyz.OP_MODE != ControlOpMode.Idle) || (tbl.OP_MODE != ControlOpMode.Idle) || (otcxyz.AB_OPMODE != ControlOpMode.Idle))
                {
                    Thread.Sleep(500);
                }
                tbl.AUTOCENTER_SW = false;
            }
        }

        /****************   Detector Bucky methods **************/
        public void WSDEject()
        {
            if (isPowerOn)
            {
                wsd.EjectDetector();
            }
        }

        public void WSDInsert(string detectorId)
        {
            if (isPowerOn)
            {
                wsd.InsertDetector(byte.Parse(detectorId));
            }
        }

        public void TBLEject()
        {
            if (isPowerOn)
            {
                tbl.EjectDetector();
            }
        }

        public void TBLInsert(string detectorId)
        {
            if (isPowerOn)
            {
                tbl.InsertDetector(byte.Parse(detectorId));
            }
        }

        /****************  Bucky Grid methods **************/
        public void TBLSetGrid(string gridType)
        {
            if (isPowerOn)
            {
                if (gridType == "Near")
                {
                    tbl.SetGrid(true);
                }
                else
                {
                    tbl.SetGrid(false);
                }
            }
            
        }

        public void WSDSetGrid(string gridType)
        {
            if (isPowerOn)
            {
                if (gridType == "Not Present")
                {
                    wsd.ChangeGridType(GridType.NotPresent);
                }
                if (gridType == "Multifocal")
                {
                    wsd.ChangeGridType(GridType.Type1);
                }
                if (gridType == "Near")
                {
                    wsd.ChangeGridType(GridType.Type2);
                }
                if (gridType == "Far")
                {
                    wsd.ChangeGridType(GridType.Type3);
                }
                if (gridType == "Near-Ped")
                {
                    wsd.ChangeGridType(GridType.Type4);
                }
                if (gridType == "Custom1")
                {
                    wsd.ChangeGridType(GridType.Type5);
                }
                if (gridType == "Custom2")
                {
                    wsd.ChangeGridType(GridType.Type6);
                }
                if (gridType == "Custom3")
                {
                    wsd.ChangeGridType(GridType.Type7);
                }
            }
        }
        #endregion
    }
}
