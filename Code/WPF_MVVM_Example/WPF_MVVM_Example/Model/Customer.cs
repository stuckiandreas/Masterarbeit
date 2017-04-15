using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WPF_MVVM_Example.Model
{
    public class Customer
    {
        private String _CustomerName;

        public String CustomerName;

        private double _Amount;

        public double Amount;

        private String _Married;

        public String Married;

        private double _Tax;
        public double Tax
        {
            get { return _Tax; }
        }

        public void CalculateTax()
        {
            if (_Amount > 2000)
            {
                _Tax = 20;
            }
            else if (_Amount > 1000)
            {
                _Tax = 10;
            }
            else
            {
                _Tax = 5;
            }
        }
    }
}
