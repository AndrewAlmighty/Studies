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

        private int m_state = 0;
        private double m_price = 0;
        private String m_productList = "";
        public MainWindow()
        {
            InitializeComponent();
            InfoTextBox.Text = "";
            
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

        private void updateTextBox(String name)
        {
            InfoTextBox.Text = name;

            if (name == "obrazek_0")
            {
                //czekolada
                InfoTextBox.Text = "Czekolada\nCena za 100g: 4.99zł\n";
                m_state = 1;
            }

            else if (name == "obrazek_1")
            {
                //banany
                InfoTextBox.Text = "Banany\nCena za 1kg: 3.99zł\n";
                m_state = 2;
            }

            else if (name == "obrazek_2")
            {
                //belwedere
                InfoTextBox.Text = "Belwedere\nCena za 0,5l: 99zł\n";
                m_state = 3;
            }

            else if (name == "obrazek_3")
            {
                //laysy
                InfoTextBox.Text = "Lays\nCena za 200g: 5.99zł\n";
                m_state = 4;
            }

            else if (name == "obrazek_4")
            {
                //tatra
                InfoTextBox.Text = "Tatra\nCena za 0,5l: 2,99zł\n";
                m_state = 5;
            }
        }

        private void StackPanel_OnMouseUp(object sender, RoutedEventArgs e)
        {
            Button btn = (Button)e.OriginalSource;
            updateTextBox(btn.Name);
        }

        private void AddClicked(object sender, RoutedEventArgs e)
        {
            InfoTextBox.Text = "";

            if (m_state == 0)
                return;

            else if (m_state == 1)
            {
                m_price += 4.99;
                m_state = 0;
                m_productList += "Czekolada\n";
            }

            else if (m_state == 2)
            {
                m_price += 3.99;
                m_state = 0;
                m_productList += "Banany\n";
            }

            else if (m_state == 3)
            {
                m_price += 99;
                m_state = 0;
                m_productList += "Belwedere\n";
            }

            else if (m_state == 4)
            {
                m_price += 5.99;
                m_state = 0;
                m_productList += "Lays\n";
            }

            else if (m_state == 5)
            {
                m_price += 2.99;
                m_state = 0;
                m_productList += "Tatra\n";
            }
        }

        private void SumClicked(object sender, RoutedEventArgs e)
        {
            InfoTextBox.Text = "Podsumowanie\n" + m_productList + "Koszt całkowity:" + m_price.ToString() + " zł";
            m_price = 0;
            m_productList = "";
        }
    }
}
