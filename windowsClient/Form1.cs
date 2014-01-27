using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;

namespace WinClient
{
    public partial class Form1 : Form
    {
        private Form obj;

        public Form1()
        {
            InitializeComponent();
            Control.CheckForIllegalCrossThreadCalls = false;
            this.obj = this;
        }

        private void SendCallback(IAsyncResult ar)
        {
            try
            {
                /* retrieve the SocketStateObject */
                SocketStateObject state = (SocketStateObject)ar.AsyncState;
                Socket socketFd = state.m_SocketFd;


                InfoBox.AppendText("Shutdown sent!\n");

                /* end connection */
                socketFd.Shutdown(SocketShutdown.Both);
                socketFd.Close();

            }
            catch (Exception exc)
            {
                MessageBox.Show("Exception:\t\n" + exc.Message.ToString());
                InfoBox.AppendText("Exception occured!\n");
            }
        }

        private void ConnectCallback(IAsyncResult ar)
        {
            try
            {
                /* retrieve the socket from the state object */
                Socket socketFd = (Socket)ar.AsyncState;

                /* complete the connection */
                socketFd.EndConnect(ar);

                /* create the SocketStateObject */
                SocketStateObject state = new SocketStateObject();
                state.m_SocketFd = socketFd;

                InfoBox.AppendText("Sending shutdown signal...\n");

                byte[] byteData = Encoding.ASCII.GetBytes("shutdown");

                /* begin sending the data */
                //socketFd.BeginReceive(state.m_DataBuf, 0, SocketStateObject.BUF_SIZE, 0, new AsyncCallback(ReceiveCallback), state);
                socketFd.BeginSend(byteData, 0, byteData.Length, 0, new AsyncCallback(SendCallback), state);
            }
            catch (Exception exc)
            {
                MessageBox.Show("Exception:\t\n" + exc.Message.ToString());
                InfoBox.AppendText("Exception occured!\n");
            }
        }

        private void buttonShutdown_Click(object sender, EventArgs e)
        {
            try
            {
                InfoBox.AppendText("Connecting to host...\n");

                IPAddress ip = IPAddress.Parse(textIP.Text.ToString());
                Int32 port = Convert.ToInt32(textPortShut.Text);
                Socket socketFd = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

                socketFd.BeginConnect(ip, port, new AsyncCallback(ConnectCallback), socketFd);
            }
            catch (Exception exc)
            {
                MessageBox.Show("Exception:\t\n" + exc.Message.ToString());
                InfoBox.AppendText("Exception occured!\n");
            }
        }

        private void SendToCallback(IAsyncResult ar)
        {
            try
            {
                /* retrieve the SocketStateObject */
                SocketStateObject state = (SocketStateObject)ar.AsyncState;
                Socket socketFd = state.m_SocketFd;

                InfoBox.AppendText("WOL sent!\n");

                /* end connection */
                socketFd.Close();
            }
            catch (Exception exc)
            {
                MessageBox.Show("Exception:\t\n" + exc.Message.ToString());
                InfoBox.AppendText("Exception occured!\n");
            }
        }

        private void buttonWOL_Click(object sender, EventArgs e)
        {
            try
            {
                string macText = textMAC.Text.Replace(":", "");
                byte[] mac = new byte[6];
                for (int i = 0; i < 6; i++)
                {
                    mac[i] = Convert.ToByte(macText.Substring(i * 2, 2), 16); // TODO
                    Console.WriteLine("{0}, {1}, {2}", i * 2, macText.Substring(i * 2, 2), mac[i]);
                }

                IPAddress ip = IPAddress.Parse(textIPBroadcast.Text.ToString());
                Int32 port = Convert.ToInt32(textPort.Text);

                Socket socketFd = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                IPEndPoint endPoint = new IPEndPoint(ip, port);

                // Prepare magic packet
                byte[] packet = new byte[17 * 6];
                // 6 times 0xFF.
                for (int i = 0; i < 6; i++)
                    packet[i] = 0xFF;
                // 16 times MAC address
                for (int i = 1; i <= 16; i++)
                    for (int j = 0; j < 6; j++)
                        packet[i * 6 + j] = mac[j];

                /* create the SocketStateObject */
                SocketStateObject state = new SocketStateObject();
                state.m_SocketFd = socketFd;

                InfoBox.AppendText("Sending WOL packet...\n");

                /* begin sending the data */
                //socketFd.BeginReceive(state.m_DataBuf, 0, SocketStateObject.BUF_SIZE, 0, new AsyncCallback(ReceiveCallback), state);
                socketFd.BeginSendTo(packet, 0, packet.Length, 0, endPoint, new AsyncCallback(SendToCallback), state);
            }
            catch (Exception exc)
            {
                MessageBox.Show("Exception:\t\n" + exc.Message.ToString());
                InfoBox.AppendText("Exception occured!\n");
            }
        }
    }

    public class SocketStateObject
    {
        public const int BUF_SIZE = 1024;
        public byte[] m_DataBuf = new byte[BUF_SIZE];
        public StringBuilder m_StringBuilder = new StringBuilder();
        public Socket m_SocketFd = null;
    }
}