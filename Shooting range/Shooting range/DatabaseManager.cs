using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace Shooting_range
{
    class DatabaseManager
    {
        private SqlConnection m_sqlConnection;
        public DatabaseManager()
        {
            m_sqlConnection = new SqlConnection("Data Source=DESKTOP-3PDS2I3\\SQLEXPRESS;Initial Catalog=ShootingRange;Integrated Security=True");
            m_sqlConnection.Open();               
        }

        ~DatabaseManager()
        {
            m_sqlConnection.Close();
        }

        public int getAmmoQuantity(string weapon)
        {
            int value = -1;
            SqlCommand cmd;
            SqlDataReader dataReader;
            cmd = new SqlCommand("Select Quantity from Ammunition where Weapon=\'" + weapon + "\'", m_sqlConnection);
            dataReader = cmd.ExecuteReader();
            if (dataReader.Read())
                value = (int)dataReader.GetValue(0);

            dataReader.Close();
            cmd.Dispose();
            return value;
        }
    }
}

/* 
 *            SqlCommand cmd;
               SqlDataReader dataReader;
               cmd = new SqlCommand("Select * from Ammunition", m_sqlConnection);
 
               dataReader.Close();
               cmd.Dispose();
 */
