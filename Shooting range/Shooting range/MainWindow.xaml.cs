using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Shooting_range
{
    /// <summary>
    /// Logika interakcji dla klasy MainWindow.xaml
    /// </summary>
    /// 

    public partial class MainWindow : Window
    {
        private DatabaseManager m_dbManager;

        public MainWindow()
        {
            InitializeComponent();
            m_dbManager = new DatabaseManager();
            int x = m_dbManager.getAmmoQuantity("AK");
            System.Console.WriteLine("-----");
            System.Console.WriteLine(x);
            System.Console.WriteLine("-----");
        }
    }
}
