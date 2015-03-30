using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PCUSimulationNS;

namespace PCUTsetConsle
{
    class Program
    {
        static void Main(string[] args)
        {

            PCUSimulator pcu = new PCUSimulator("COM18", "COM19");
            pcu.Connect();
            Console.Read();
        }
    }
}
