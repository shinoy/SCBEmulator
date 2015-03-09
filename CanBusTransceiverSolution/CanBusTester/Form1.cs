using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using CanBusChannelNS;
using SCBProtocalWrapper;
using PositionerSimulationNS;
using PCUSimulationNS;
using System.Threading;

namespace CanBusTester
{
    public partial class Form1 : Form
    {
        private CanBusChannelCls bus = new CanBusChannelCls();

        private OTCXYZControllerCLS otcxyz = null;

        private WSDControllerCLS wsd = null;

        private TBLControllerCLS tbl = null;

        private CollimatorCLS col = null;

        private PCUSimulator pcu = new PCUSimulator("COM18", "COM19");

        public delegate void RefreshDelegate();
        
        public Form1()
        {
            InitializeComponent();
            bus.Connect();
            pcu.Connect();
        }

        private void RefreshData()
        {

            if (InvokeRequired)
            {
                Invoke(new RefreshDelegate(RefreshData));
            }
            else
            {
                    this.textBox1.Text = otcxyz.X_POS.ToString();
                    this.textBox2.Text = otcxyz.Y_POS.ToString();
                    this.textBox3.Text = otcxyz.Z_POS.ToString();
                    this.textBox4.Text = otcxyz.A_POS.ToString();
                    this.textBox5.Text = otcxyz.B_POS.ToString();
                    this.textBox6.Text = wsd.X_POS.ToString();
                    this.textBox7.Text = wsd.Y_POS.ToString();
                    this.textBox8.Text = wsd.Z_POS.ToString();
                    this.textBox9.Text = tbl.X_POS.ToString();
                    this.textBox10.Text = tbl.Y_POS.ToString();
                    this.textBox11.Text = tbl.Z_POS.ToString();
               
            }
         

        }

        private void button1_Click(object sender, EventArgs e)
        {
            otcxyz = new OTCXYZControllerCLS(bus);
            wsd = new WSDControllerCLS(bus);
            tbl = new TBLControllerCLS(bus);
            col = new CollimatorCLS(bus);
           
            bus.StartBus();
            otcxyz.PowerUp();
            wsd.PowerUp();
            tbl.PowerUp();
            col.PowerUp();
            pcu.BuckyStartEvent += tbl.BuckyStartEventHandler;
            this.textBox1.Text = otcxyz.X_POS.ToString();
            this.textBox2.Text = otcxyz.Y_POS.ToString();
            this.textBox3.Text = otcxyz.Z_POS.ToString();

            otcxyz.PositionUpdateEvent += new PositonUpdateDelegate(RefreshData);

            this.button2.Enabled = true;
            this.button1.Enabled = false;
            this.button3.Enabled = true;
            this.button4.Enabled = true;
            this.button5.Enabled = true;
            this.button6.Enabled = true;
            this.button7.Enabled = true;
            this.button8.Enabled = true;
            this.button9.Enabled = true;
            this.button10.Enabled = true;
            this.button11.Enabled = true;
            this.button12.Enabled = true;
            this.button13.Enabled = true;
            this.button14.Enabled = true;
            this.button19.Enabled = true;
            this.button20.Enabled = true;

            this.checkBox1.Enabled = true;
            this.checkBox2.Enabled = true;
            this.checkBox3.Enabled = true;
            this.checkBox4.Enabled = true;
            this.checkBox5.Enabled = true;
            this.checkBox6.Enabled = true;
            


        }

        private void button2_Click(object sender, EventArgs e)
        {

            this.button1.Enabled = true;
            this.button2.Enabled = false;
            this.button3.Enabled = false;
            this.button4.Enabled = false;
            this.button5.Enabled = false;
            this.button6.Enabled = false;
            this.button7.Enabled = false;
            this.button8.Enabled = false;
            this.button9.Enabled = false;
            this.button10.Enabled = false;
            this.button11.Enabled = false;
            this.button12.Enabled = false;
            this.button13.Enabled = false;
            this.button14.Enabled = false;
            this.button19.Enabled = false;
            this.button20.Enabled = false;
            this.checkBox1.Enabled = false;
            this.checkBox2.Enabled = false;
            this.checkBox3.Enabled = false;
            this.checkBox4.Enabled = false;
            this.checkBox5.Enabled = false;
            this.checkBox6.Enabled = false;

            otcxyz.PowerOff();
            wsd.PowerOff();
            tbl.PowerOff();
            pcu.BuckyStartEvent -= tbl.BuckyStartEventHandler;
            bus.StopBus();
            

           
        }

        private void button3_Click(object sender, EventArgs e)
        {
           
        }

        private void button4_Click(object sender, EventArgs e)
        {
            bus.TestSend(true);
        }

        private void button3_Click_1(object sender, EventArgs e)
        {
            if (otcxyz != null)
            {
                otcxyz.X_POS = double.Parse(this.textBox1.Text);
                otcxyz.Y_POS = double.Parse(this.textBox2.Text);
                otcxyz.Z_POS = double.Parse(this.textBox3.Text);
            }
        }

        private void button3_Click_2(object sender, EventArgs e)
        {
            otcxyz.XStepUp();
            RefreshData();
        }

        private void button4_Click_1(object sender, EventArgs e)
        {
            otcxyz.XStepDown();
            RefreshData();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            otcxyz.YStepUp();
            RefreshData();
        }

        private void button6_Click(object sender, EventArgs e)
        {
            otcxyz.YStepDown();
            RefreshData();
        }

        private void button7_Click(object sender, EventArgs e)
        {
            otcxyz.ZStepUp();
            RefreshData();
        }

        private void button8_Click(object sender, EventArgs e)
        {
            otcxyz.ZStepDown();
            RefreshData();
        }

        private void button9_Click(object sender, EventArgs e)
        {
            otcxyz.AStepUp();
            RefreshData();
        }

        private void button10_Click(object sender, EventArgs e)
        {
            otcxyz.AStepDown();
            RefreshData();
        }

        private void button11_Click(object sender, EventArgs e)
        {
            otcxyz.BStepUp();
            RefreshData();
        }

        private void button12_Click(object sender, EventArgs e)
        {
            otcxyz.BStepDown();
            RefreshData();
        }

        private void label5_Click(object sender, EventArgs e)
        {

        }


        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked == true)
            {
                otcxyz.XRelBtnPress();
            }
            else
            {
                otcxyz.XRelBtnRelease();
            }
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox2.Checked == true)
            {
                otcxyz.YRelBtnPress();
            }
            else
            {
                otcxyz.YRelBtnRelease();
            }
        }

        private void checkBox3_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox3.Checked == true)
            {
                otcxyz.ZRelBtnPress();
            }
            else
            {
                otcxyz.ZRelBtnRelease();
            }
        }

        private void checkBox4_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox4.Checked == true)
            {
                otcxyz.XYZRelBtnPress();
            }
            else
            {
                otcxyz.XYZRelBtnRelease();
            }
        }

        private void checkBox5_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox5.Checked == true)
            {
                otcxyz.ARelBtnPress();
            }
            else
            {
                otcxyz.ARelBtnRelease();
            }
        }

        private void checkBox6_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox6.Checked == true)
            {
                otcxyz.BRelBtnPress();
            }
            else
            {
                otcxyz.BRelBtnRelease();
            }
        }

        private void checkBox7_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox7.Checked == true)
            {
                tbl.FLOAT_SW = true;
            }
            else
            {
                tbl.FLOAT_SW = false;
            }
        }



        private void checkBox8_CheckedChanged(object sender, EventArgs e)
        {
            wsd.keypad.AUTO_POS_SW = checkBox8.Checked;
        }

        private void button13_Click(object sender, EventArgs e)
        {
            tbl.XStepUp();
            RefreshData();
        }

        private void button14_Click(object sender, EventArgs e)
        {
            tbl.XStepDown();
            RefreshData();
        }

        private void button19_Click(object sender, EventArgs e)
        {
            wsd.ZStepDown();
            RefreshData();
        }

        private void button20_Click(object sender, EventArgs e)
        {
            wsd.ZStepUp();
            RefreshData();
        }

        private void checkBox9_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox9.Checked)
            {
                wsd.ZRelBtnPress();    
            }
            else
            {
                wsd.ZRelBtnRelease();
            }
        }

        private void DetectorSelectBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.DetectorSelectBox.SelectedIndex == 0)
            {
                tbl.EjectDetector();
                wsd.InsertDetector(0x01);
               
            }
            if (this.DetectorSelectBox.SelectedIndex == 1)
            {
                wsd.EjectDetector();
                tbl.InsertDetector(0x01);
            }
        }

        private void checkBox10_CheckedChanged(object sender, EventArgs e)
        {
         
                tbl.AUTOCENTER_SW = checkBox10.Checked;    
       
        }

        private void checkBox11_CheckedChanged(object sender, EventArgs e)
        {
            wsd.keypad.AUTO_CENTER_SW = checkBox11.Checked;
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            wsd.ChangeGridType((GridType)comboBox1.SelectedIndex);
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBox2.SelectedIndex != 0)
            {
                tbl.SetGrid(true);
            }
            else
            {
                tbl.SetGrid(false);
            }
        }
           
       
    }
}
