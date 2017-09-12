//-----------------------------------------------------------------------
// <copyright file="BasisViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2015 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

// Only references to the Model are allowed. References to Views are not allowed
namespace VersionManagement.ViewModels
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

        /// <summary>
        /// Initializes a new instance of the <see cref="BasisViewModel"/> class.
        /// </summary>
        /// <param name="currentEvents">The current events.</param>
        protected BasisViewModel(ViewModelEvents currentEvents)
        {
            this.events = currentEvents;
        }

        /// <summary>
        /// Property Changed Event for Bindings
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// Initializes this instance.
        /// </summary>
        public virtual void Init()
        {
        }

        /// <summary>
        /// Subscribes the events.
        /// </summary>
        public virtual void SubscribeEvents()
        {
        }

        /// <summary>
        /// Unsubscribes the events.
        /// </summary>
        public virtual void UnsubscribeEvents()
        {
        }

        /// <summary>
        /// Called when [handle error].
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="e">The <see cref="ErrorHandlerEventArgs"/> instance containing the event data.</param>
        public void OnHandleError(object sender, ErrorHandlerEventArgs e)
        {
            this.events.OnHandleError(sender, e);
        }

        /// <summary>
        /// Called when [user feedback].
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="e">The <see cref="UserFeedbackEventArgs"/> instance containing the event data.</param>
        public void OnUserFeedback(object sender, UserFeedbackEventArgs e)
        {
            this.events.OnUserFeedback(sender, e);
        }

        /// <summary>
        /// Property Changed Event helper method
        /// </summary>
        /// <param name="propertyName">Name of the changed Property</param>
        protected void OnPropertyChanged(string propertyName)
        {
            if (this.PropertyChanged != null)
                this.PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
