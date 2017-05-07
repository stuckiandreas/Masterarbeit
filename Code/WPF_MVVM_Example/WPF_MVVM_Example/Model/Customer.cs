using System;
using System.Collections.Generic;
using System.IO.Packaging;
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

        public Customer(String CustomerName, double Amount, String Married)
        {
            this.CustomerName = CustomerName;
            this.Amount = Amount;
            this.Married = Married;
        }


        public double Tax
        {
            get { return _Tax; }
            set { Tax = _Tax; }
        }

        public void CalculateTax()
        {
            if (Amount > 2000)
            {
                _Tax = 20;
            }
            else if (Amount > 1000)
            {
                _Tax = 10;
            }
            else
            {
                _Tax = 5;
            }
        }
        public bool IsValid()
        {
            if (Amount == 0)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
}
