using DevExpress.Xpf.Grid;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Threading;

namespace ETIC2
{
    class MasterGridHelper
    {
        public static GridControl GetDetailGrid(DependencyObject obj)
        {
            return (GridControl)obj.GetValue(DetailGridProperty);
        }
        public static void SetDetailGrid(DependencyObject obj, GridControl value)
        {
            obj.SetValue(DetailGridProperty, value);
        }
        public static readonly DependencyProperty DetailGridProperty =
            DependencyProperty.RegisterAttached("DetailGrid", typeof(GridControl), typeof(MasterGridHelper), new PropertyMetadata(null));

        public static string GetSearchString(DependencyObject obj)
        {
            return (string)obj.GetValue(SearchStringProperty);
        }
        public static void SetSearchString(DependencyObject obj, string value)
        {
            obj.SetValue(SearchStringProperty, value);
        }
        public static readonly DependencyProperty SearchStringProperty =
            DependencyProperty.RegisterAttached("SearchString", typeof(string), typeof(MasterGridHelper),
            new FrameworkPropertyMetadata(null,
                FrameworkPropertyMetadataOptions.Inherits,
                new PropertyChangedCallback(
                (obj, e) =>
                {
                    var view = obj as TableView;
                    if (view == null || !view.HasDetailViews) return;
                    var masterGrid = view.Grid;
                    if (masterGrid == null) return;

                    // expand all details
                    for (int i = 0; i < masterGrid.VisibleRowCount; i++)
                    {
                        var masterRowHandle = masterGrid.GetRowHandleByVisibleIndex(i);
                        masterGrid.ExpandMasterRow(masterRowHandle);
                    }

                    // collapse empty details with a delay
                    Dispatcher.CurrentDispatcher.BeginInvoke(new Action(() => CollapseEmptyDetails(masterGrid)), DispatcherPriority.Render);
                })));

        static void CollapseEmptyDetails(GridControl masterGrid)
        {
            for (int i = 0; i < masterGrid.VisibleRowCount; i++)
            {
                var masterRowHandle = masterGrid.GetRowHandleByVisibleIndex(i);
                var masterRowState = masterGrid.GetRowState(masterRowHandle, false);
                if (masterRowState != null)
                {
                    var detailGrid = MasterGridHelper.GetDetailGrid(masterRowState);
                    if (detailGrid != null && detailGrid.VisibleRowCount == 0)
                    {
                        masterGrid.CollapseMasterRow(masterRowHandle);
                    }
                }
            }
        }
    }
}
