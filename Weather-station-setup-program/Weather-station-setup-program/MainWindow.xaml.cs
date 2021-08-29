using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
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

namespace Weather_station_setup_program
{
    public partial class MainWindow : Window
    {
        SerialPort serial_port;
        string[] available_ports;

        public MainWindow()
        {
            InitializeComponent();
            available_ports = SerialPort.GetPortNames();//vettore contenente la lista delle porte seriali disponibili

            foreach(string serial_port in available_ports)
            {
                cmb_serial_port.Items.Add(serial_port);
            }
        }

        private void btn_go_Click(object sender, RoutedEventArgs e)
        {
            //apro la comunicazione seriale
            string serial_port_name = available_ports[cmb_serial_port.SelectedIndex];
            serial_port = new SerialPort(serial_port_name,115200);
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
                MessageBox.Show("Everything went well, restart your station");
            }
            catch {
                MessageBox.Show("Serial port not available");
            }
        }
    }
}
