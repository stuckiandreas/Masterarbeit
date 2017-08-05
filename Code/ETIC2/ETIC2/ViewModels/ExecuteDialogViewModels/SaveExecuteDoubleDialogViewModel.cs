//-----------------------------------------------------------------------
// <copyright file="SaveExecuteDoubleDialogViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 201^7 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels.ExecuteDialogViewModels
{
    using Events;
    using InputViewModels;
    using Model.Application;

    /// <summary>
    /// ViewModel for saving two inputs
    /// </summary>
    /// <seealso cref="ETIC2.ViewModels.ExecuteDialogViewModels.ExecuteDialogViewModel" />
    public class SaveExecuteDoubleDialogViewModel : ExecuteDialogViewModel
    {
        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        /// <summary>
        /// References to the model
        /// </summary>
        private ETIC2Model etic2Model;

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
            ETIC2Model etic2Model,
            string label1,
            string label2,
            string textInput1,
            string textInput2)
            : base(viewModelEvents, buttonText)
        {
            this.viewModelEvents = viewModelEvents;
            this.etic2Model = etic2Model;
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
        }
    }
}
