using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace SWP_LAB3
{
    class MyItem
    {
        private String m_name;
        private String m_description;
        private String m_price;
        private BitmapImage m_bitmap;
        private Image m_image;
        
        public MyItem(String name, String description, String price, String path_to_image)
        {
            m_name = name;
            m_description = description;
            m_price = price;
            m_bitmap = new BitmapImage(new Uri(path_to_image, UriKind.Relative));

        }

        public String getName()
        {
            return m_name;
        }

        public String getDescription()
        {
            return m_description;
        }

        public String getPrice()
        {
            return m_price;
        }
    }
}
