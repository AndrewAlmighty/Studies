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

        public bool checkIfDayAndHourIsFree(String Date, int hour)
        {
            bool isFree = false;
            SqlCommand cmd;
            SqlDataReader dataReader;
            cmd = new SqlCommand("SELECT * FROM TimeTable WHERE OnDay = \'" + Date + 
                "\' AND OnHour = " + hour.ToString(), m_sqlConnection);
            dataReader = cmd.ExecuteReader();

            if (dataReader.Read())
                isFree = true;

            dataReader.Close();
            cmd.Dispose();
            return isFree;
        }

        public void insertToTimeTable(String Date, int hour)
        {
            SqlCommand cmd;
            SqlDataAdapter dataAdapter = new SqlDataAdapter();
            cmd = new SqlCommand("INSERT INTO TimeTable (OnDay, OnHour) VALUES (\'" + Date + 
                "\', " + hour.ToString() + ")", m_sqlConnection);
            dataAdapter.UpdateCommand = cmd;
            dataAdapter.UpdateCommand.ExecuteNonQuery();
            cmd.Dispose();
        }

        public void updateAmmoQuantity(string weapon, int ammo)
        {
            SqlCommand cmd;
            SqlDataAdapter dataAdapter = new SqlDataAdapter();
            cmd = new SqlCommand("Update Ammunition SET Quantity = "+ ammo.ToString() +" from Ammunition where Weapon=\'" + weapon + "\'", m_sqlConnection);
            dataAdapter.UpdateCommand = cmd;
            dataAdapter.UpdateCommand.ExecuteNonQuery();
            cmd.Dispose();
        }
    }
}