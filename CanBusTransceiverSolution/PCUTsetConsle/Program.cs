using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using PCUSimulationNS;

namespace PCUTsetConsle
{
    class Program
    {
        static void Main(string[] args)
        {

            PCUSimulator pcu = new PCUSimulator();
            pcu.Connect();
            Console.Read();
        }
    }
}
