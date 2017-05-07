using System;
using System.ComponentModel;
using System.Windows.Input;
using System.Windows.Media;
using DevExpress.Mvvm.DataAnnotations;
using DevExpress.Mvvm;
using WPF_MVVM_Example.Commands;
using WPF_MVVM_Example.Model;

namespace WPF_MVVM_Example.ViewModels
{
    [POCOViewModel]
    public class CustomerViewModel : INotifyPropertyChanged
    {
        private Customer obj = new Customer("Andreas Stucki", 2000, "Not Married");
        public event PropertyChangedEventHandler PropertyChanged;
        public virtual void RaisePropertyChanged(string propertyName) { }

        public CustomerViewModel()
        {
            BtnClick = new ActionCommand(OnBtnClick, OnCanExecuteBtnClick);
        }

        public ICommand BtnClick
        {
            get;
            private set;
        }

        public bool OnCanExecuteBtnClick(object parameter)
        {
            return obj.IsValid();
        }

        private void OnBtnClick(object parameter)
        {
            obj.CalculateTax();

            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs("TxtTax"));
            }
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

        public string TxtTax
        {
            get { return Convert.ToString(obj.Tax); }
            set
            {
                obj.Tax = Convert.ToDouble(value);
                OnPropertyChanged("TxtTax");
            }
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

        protected void OnPropertyChanged(string propertyName)
        {
            if (this.PropertyChanged != null)
                this.PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}