using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Input;

namespace SimpleNavigation
{
    public class NavigationViewModel : INotifyPropertyChanged

    {

        public ICommand EmpCommand { get; set; }

        public ICommand DeptCommand { get; set; }

        private object selectedViewModel;

        private readonly ObservableCollection<BaseViewModel> allViewModels;

        public object SelectedViewModel

        {

            get { return selectedViewModel; }

            set { selectedViewModel = value; OnPropertyChanged("SelectedViewModel"); }

        }



        public NavigationViewModel()

        {
            allViewModels = new ObservableCollection<BaseViewModel>();
            allViewModels.Add(new EmployeeViewModel());
            allViewModels.Add(new DepartmentViewModel());

            EmpCommand = new BaseCommand(OpenEmp);

            DeptCommand = new BaseCommand(OpenDept);

        }

        private void OpenEmp(object obj)

        {
            
            SelectedViewModel = allViewModels.Where(x => x is EmployeeViewModel).Single();

        }

        private void OpenDept(object obj)

        {
            SelectedViewModel = allViewModels.Where(x => x is DepartmentViewModel).Single();

        }

        public event PropertyChangedEventHandler PropertyChanged;

        private void OnPropertyChanged(string propName)

        {

            if (PropertyChanged != null)

            {

                PropertyChanged(this, new PropertyChangedEventArgs(propName));

            }

        }

    }
}
