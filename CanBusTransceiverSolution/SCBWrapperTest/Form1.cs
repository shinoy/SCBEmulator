using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using EmuTest;

namespace SCBWrapperTest
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Assembly assembly = Assembly.LoadFrom(@"./EmuTest.exe");
            Type SCBType = assembly.GetType("EmuTest.SCBWrapper", true);
            SCBWrapper controller = (SCBWrapper)Activator.CreateInstance(SCBType);
        }
    }
}
