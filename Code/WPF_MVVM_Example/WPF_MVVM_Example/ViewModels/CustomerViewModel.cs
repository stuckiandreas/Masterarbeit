﻿using System;
using System.Windows.Input;
using System.Windows.Media;
using DevExpress.Mvvm.DataAnnotations;
using DevExpress.Mvvm;
using WPF_MVVM_Example.Model;

namespace WPF_MVVM_Example.ViewModels
{
    [POCOViewModel]
    public class CustomerViewModel
    {
        private Customer obj = new Customer("Andreas Stucki", 2000, "Not Married");

        public CustomerViewModel()
        {
            TaxCommand = new ActionCommand(OnTaxCommand, OnCanExecuteTaxCommand);
        }


        public string TxtCustomerName
        {
            get { return obj.CustomerName; }
            set { obj.CustomerName = value; }
        }

        public string TxtAmount
        {
            get { return Convert.ToString(obj.Amount); }
            set { obj.Amount = Convert.ToDouble(value); }
        }


        public string LblAmountColor
        {
            get
            {
                if (obj.Amount > 2000)
                {
                    return "Blue";
                }
                else if (obj.Amount > 1500)
                {
                    return "Red";
                }
                return "Yellow";
            }
        }

        public bool IsMarried
        {
            get
            {
                if (obj.Married == "Married")
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }

        }
        public void Calculate()
        {
            obj.CalculateTax();
        }

        public ICommand TaxCommand
        {
            get;
            private set;
        }
    }
}