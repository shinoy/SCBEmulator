using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using PCUSimulationNS;
using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting.Channels.Tcp;

namespace PDUTester
{
    public partial class Form1 : Form
    {
        PCUSimulator pcu = null;

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            pcu = getPCUService();
            if (pcu != null)
            {
                pcu.EnAutoExp();
            }
           
        }

        private PCUSimulator getPCUService()
        {
            try
            {
                ChannelServices.RegisterChannel(new TcpClientChannel(), false);
                PCUSimulator simulator = (PCUSimulator)Activator.GetObject(typeof(PCUSimulator), "tcp://localhost:6688/PCUSimulater");
                simulator.TestRomote();
                if (simulator != null)
                {

                    MessageBox.Show("成功获取remote PCU");
                }
                return simulator;

            }
            catch (Exception ex) 
            {
                MessageBox.Show("获取失败：");
                return null; 
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (pcu != null)
            {
                pcu.Pre();
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (pcu != null)
            {
                pcu.Expose();
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (pcu != null)
            {
                pcu.Release();
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (pcu != null)
            {
                pcu.TestRomote();
            }
        }
    }
}
