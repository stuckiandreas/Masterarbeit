using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrderList
{
    public class RandomStringHelper
    {
        static Random rnd = new Random();
        public static Random Random { get { return rnd; } }

        public static string GetRandomString(int min = 6, int max = 20)
        {
            StringBuilder strb = new StringBuilder();
            strb.Append((char)rnd.Next(0x41, 0x5A));

            int length = rnd.Next(min, max);
            for (int i = 0; i < length - 1; i++)
                strb.Append((char)rnd.Next(0x61, 0x7A));

            return strb.ToString();
        }
    }
}
