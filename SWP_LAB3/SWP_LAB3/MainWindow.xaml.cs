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
using Microsoft.Kinect;
using Microsoft.Kinect.Wpf.Controls;

namespace SWP_LAB3
{
    /// <summary>
    /// Logika interakcji dla klasy MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
       
        private StackPanel m_stackPanel;
        
        private event MouseButtonEventHandler m_click;
        private BitmapImage[] m_bitMaps = new BitmapImage[5];
        private Button[] m_images = new Button[5];
        private String[] m_imageNames = { "czekolada.jpg", "banany.jpg", "belwedere.jpg", "lays.jpg", "tatra.jpg" };
        public MainWindow()
        {
            InitializeComponent();
            
            m_stackPanel = new StackPanel();
            m_stackPanel.MouseLeftButtonUp += new MouseButtonEventHandler(StackPanel_OnMouseUp);
            m_stackPanel.Orientation = Orientation.Horizontal;
            m_stackPanel.HorizontalAlignment = HorizontalAlignment.Left;
            m_stackPanel.VerticalAlignment = VerticalAlignment.Top;
            for (int i = 0; i < 5; i++)
            {
                m_bitMaps[i] = new BitmapImage(new Uri(".\\Images\\" + m_imageNames[i], UriKind.Relative));
                m_images[i] = new Button();
                m_images[i].Name = "obrazek_" + i.ToString();
                Image img = new Image();
                img.Source = m_bitMaps[i];
                m_images[i].Content = img;
                m_images[i].Width = 200;
                m_images[i].Height = 200;
                m_images[i].Click += StackPanel_OnMouseUp;
                m_stackPanel.Children.Add(m_images[i]);
            }

            MainScrollViewer.Content = m_stackPanel;
        }

        private void StackPanel_OnMouseUp(object sender, RoutedEventArgs e)
        {
            InfoTextBox.Text = "dziala";
        }
    }
}
