//-----------------------------------------------------------------------
// <copyright file="BasisViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

// Only references to the Model are allowed. References to Views are not allowed
namespace ETIC2.ViewModels
{
    using System.ComponentModel;
    using Events;
    using Events.EventArgs.Error;
    using Events.EventArgs.UserFeedback;

    /// <summary>
    /// BasisViewModel for all ViewModels.
    /// Contains all the stuff that belongs to every ViewModel:
    /// - Logger,
    /// - Events helper class including events that are used in several view models (e.g. ErrorHandler, UserFeedbackHandler)
    /// - PropertyChanged realization,
    /// - Virtual methods for SubscribeEvents, UnsubscribeEvents and <Init/>
    /// </summary>
    public abstract class BasisViewModel : INotifyPropertyChanged
    {
        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        protected ViewModelEvents events;

        protected BasisViewModel(ViewModelEvents currentEvents)
        {
            this.events = currentEvents;
        }

        /// <summary>
        /// Property Changed Event for Bindings
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        public virtual void Init()
        {
        }

        public virtual void SubscribeEvents()
        {
        }

        public virtual void UnsubscribeEvents()
        {
        }

        public void OnHandleError(object sender, ErrorHandlerEventArgs e)
        {
            this.events.OnHandleError(sender, e);
        }

        public void OnUserFeedback(object sender, UserFeedbackEventArgs e)
        {
            this.events.OnUserFeedback(sender, e);
        }

        protected void OnPropertyChanged(string propertyName)
        {
            if (this.PropertyChanged != null)
                this.PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
