//-----------------------------------------------------------------------
// <copyright file="DetailedInformationViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels.BuglistViewModels
{
    using System;
    using ETIC2.Model.Application;
    using Events;
    using InputViewModels;
    
    /// <summary>
    /// Shows the detail information of the selected buglist item in the databaseDataGrid
    /// </summary>
    public class DetailedInformationViewModel : BasisViewModel
    {
        //Input labels
        private const string FailureTypeLabel = "Failure Type:";

        private const string StatusTypeLabel = "Status Type";

        private const string ControllerTypeLabel = "Controller Type";

        private const string HardwareIdentificationLevel1Label = "Hardware Identification Level 1";

        private const string HardwareIdentificationLevel2Label = "Hardware Identification Level 2";

        private const string BugLabel = "Bug";

        private const string CommentLabel = "Comment";

        private const string PriorityLabel = "Priority";

        private const string DateFoundLabel = "Date Found";

        private const string DateFixedLabel = "Date Fixed";

        private readonly ETIC2Model etic2Model;

        //Inputs
        private DefinedSelectionInputListViewModel failureType;

        private DefinedSelectionInputListViewModel statusType;

        private DefinedSelectionInputListViewModel controllerType;

        private DefinedSelectionInputListViewModel hardwareIdentificationLevel1;

        private DefinedSelectionInputListViewModel hardwareIdentificationLevel2;

        private TextBoxInputViewModel bug;

        private TextBoxInputViewModel comment;

        private DefinedSelectionInputListViewModel priority;

        private DateTimeInputViewModel dateFound;

        private DateTimeInputViewModel dateFixed;

        /// <summary>
        /// Reference to the database item view model. 
        /// </summary>
        private DatabaseItemViewModel databaseItemViewModel;

        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        public DetailedInformationViewModel(ViewModelEvents viewModelEvents, ETIC2Model etic2Model)
            : base(viewModelEvents)
        {
            // Create all commands
            this.NewCommand = new ActionCommand(this.OnNewCommand, this.OnCanExecuteNewCommand);
            this.SaveCommand = new ActionCommand(this.OnSaveCommand, this.OnCanExecuteSaveCommand);
            this.DeleteCommand = new ActionCommand(this.OnDeleteCommand, this.OnCanExecuteDeleteCommand);

            this.viewModelEvents = viewModelEvents;
            this.etic2Model = etic2Model;

            this.InitInputs();
        }

        /// <summary>
        /// Occurs when an item changed in the detail information view. Example when the user define a new software item.
        /// </summary>
        public event EventHandler<ItemEventArgs> ItemChangedEvent;
    }
}
