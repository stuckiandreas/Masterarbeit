using System.ComponentModel;
using System.Windows.Markup;
using DevExpress.Xpf.Core;

namespace DXSample {
    class ApplicationThemeProvider : MarkupExtension, INotifyPropertyChanged {
        
        public override object ProvideValue(System.IServiceProvider serviceProvider) {
            return this;
        }

        void ThemeManager_ApplicationThemeChanged(System.Windows.DependencyObject sender, ThemeChangedRoutedEventArgs e) {
            NotifyPropertyChanged("ApplicationThemeName");
        }

        public event PropertyChangedEventHandler PropertyChanged;
        void NotifyPropertyChanged(string propertyName) {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
