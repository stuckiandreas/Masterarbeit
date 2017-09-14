//-----------------------------------------------------------------------
// <copyright file="SaveExecuteDoubleDialogViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ViewModels.ExecuteDialogViewModels
{
    using Events;
    using InputViewModels;
    using Model;

    public class SaveExecuteDoubleDialogViewModel : ExecuteDialogViewModel
    {
        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        /// <summary>
        /// References to the model
        /// </summary>
        private VersionManagementModel versionManagementModel;

        /// <summary>
        /// The name text input view model1
        /// </summary>
        private TextInputViewModel textInputViewModel1;

        /// <summary>
        /// The name text input view model2
        /// </summary>
        private TextInputViewModel textInputViewModel2;

        public SaveExecuteDoubleDialogViewModel(
            ViewModelEvents viewModelEvents,
            string buttonText,
            VersionManagementModel versionManagementModel,
            string label1,
            string label2,
            string textInput1,
            string textInput2)
            : base(viewModelEvents, buttonText)
        {
            this.viewModelEvents = viewModelEvents;
            this.versionManagementModel = versionManagementModel;
            this.textInputViewModel1 = new TextInputViewModel(viewModelEvents, label1, textInput1);
            this.textInputViewModel2 = new TextInputViewModel(viewModelEvents, label2, textInput2);
        }

        public TextInputViewModel TextInputViewModel1
        {
            get
            {
                return this.textInputViewModel1;
            }

            set
            {
                this.textInputViewModel1 = value;
                this.OnPropertyChanged("TextInputViewModel1");
            }
        }

        public TextInputViewModel TextInputViewModel2
        {
            get
            {
                return this.textInputViewModel2;
            }

            set
            {
                this.textInputViewModel2 = value;
                this.OnPropertyChanged("TextInputViewModel2");
            }
        }

        public override void Execute()
        {
            this.versionManagementModel.SelectionItemDatabaseAccessManager.AddPropertyType(this.TextInputViewModel1.TextInput, this.TextInputViewModel2.TextInput);
        }
    }
}
