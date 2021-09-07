using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
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

namespace ClientSetup
{
    public partial class MainWindow : Window
    {
        SerialPort serial_port;
        string[] available_ports;
        string[] sensors =  { "temperature", "pressure", "humidity" };
        
        public MainWindow()
        {
            InitializeComponent();
            available_ports = SerialPort.GetPortNames();//vettore contenente la lista delle porte seriali disponibili

            foreach (string serial_port in available_ports)
            {
                cmb_serial_port.Items.Add(serial_port);
            }

            foreach(string sensor in sensors)
            {
                cmb_sensor.Items.Add(sensor);
            }
            
        }

        private void btn_go_Click(object sender, RoutedEventArgs e)
        {
            if (Convert.ToInt32(txt_min_value.Text) < Convert.ToInt32(txt_max_value.Text))
            {
                //apro la comunicazione seriale
                string serial_port_name = available_ports[cmb_serial_port.SelectedIndex];
                serial_port = new SerialPort(serial_port_name, 115200);
                try
                {
                    serial_port.Open();
                    //invio l'ssid
                    string ssid_message = "set -parameter ssid -value" + txt_ssid.Text;
                    serial_port.Write(ssid_message);
                    Thread.Sleep(500);
                    //invio la password
                    string password_message = "set -parameter password -value" + passbox_password.Password;
                    serial_port.Write(password_message);
                    string sensor_message = "set -parameter sensor -value" + sensors[cmb_sensor.SelectedIndex];
                    serial_port.Write(password_message);
                    string min_value_message = "set -parameter minvalue -value" + txt_min_value.Text;
                    serial_port.Write(password_message);
                    string max_value_message = "set -parameter maxvalue -value" + txt_max_value.Text;
                    serial_port.Write(password_message);
                    MessageBox.Show("Everything went well, restart your station");
                }
                catch
                {
                    MessageBox.Show("Serial port not available");
                }
            }
            else
            {
                MessageBox.Show("Minvalue cannot be greater than maxvalue");
            }
        }
       

        private void txt_min_value_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9]+");
            e.Handled = regex.IsMatch(e.Text);
        }

        private void txt_max_value_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9]+");
            e.Handled = regex.IsMatch(e.Text);
        }
    }
}
