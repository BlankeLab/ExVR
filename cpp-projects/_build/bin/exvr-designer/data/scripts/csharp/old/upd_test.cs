// system
using System;
using System.Collections;
using System.Reflection;
using System.Collections.Generic;
using System.Net;
using System.Text;
using System.Net.Sockets;
// unity
using UnityEngine;

namespace Ex
{

    //    if(udpTime==10){
    //		// send data via udp
    //		MsgSize		= sprintf(buffer,"[%d ;%d ;%d ;%d ;%d ;%d ;%d ;%d; %+3.3lf; %+3.3lf]",
    //                            m_send.X_Master, m_send.Y_Master, m_send.Z_Master,
    //                            m_send.X_Slave, m_send.Y_Slave, m_send.Z_Slave,
    //                            m_send.FlagTouch_Slave,
    //                            m_send.FlagTouch_Master,
    //                            m_send.SlaveForceValue,
    //                            m_send.MasterForceValue);
    //    checkUDP	= sendto(sockUDPsend, (const char*)(&buffer), sizeof(buffer),0,(SOCKADDR*)&remoteUDPsend,AddressSize);
    //		// check if data received
    //		int tmp = sizeof(remoteUDPlisten);
    //    int nbChar = recvfrom(sockUDPlisten, RecvBuf, 1024, 0, (struct sockaddr*)&remoteUDPlisten, &tmp);
    //		//GetLastError();
    //		resultRecv = nbChar;
    //		if(nbChar != SOCKET_ERROR){ // SOCKET_ERROR if no data was received -> don't change delay
    //			RecvBuf[nbChar]=0;		// close the buffer after data, because recvfrom function doesn't do it
    //			changeConditions(std::string(RecvBuf));
    //}
    //udpTime = 0;
    //	}
    //ssize_t sendto(int sockfd, const void* buf, size_t len, int flags,
    //               const struct sockaddr * dest_addr, socklen_t addrlen)

    //#define APP_PORT_SEND				5554
    //#define APP_PORT_LISTEN				5555
    //#define ADDRESS						"127.0.0.1"

    //   	remoteUDPsend.sin_family		= AF_INET;
    //remoteUDPsend.sin_addr.s_addr	= inet_addr(ADDRESS);   // send on given IP address only
    //   remoteUDPsend.sin_port			= htons(APP_PORT_SEND);

    //struct sockaddr_in
    //{
    //    short sin_family;
    //    u_short sin_port;
    //    struct in_addr sin_addr;
    //    char sin_zero[8];
    //};

    // sizeof
    //struct sockaddr
    //{
    //    u_short sa_family;              /* address family */
    //    char sa_data[14];            /* up to 14 bytes of direct address */
    //};


    public class UdpTestComponent : BaseCompiledCSharpComponent{

        public int readingPort;
        public IPAddress readingAddress = null;
        private IPEndPoint clientIpEndPoint = null;

        private Socket clientSocket = null;
        private const int bufSize = 8 * 1024;
        private byte[] buffer = new byte[bufSize];
        // ### Ex component flow functions, uncomment to use
        // # main functions
        public override bool initialize() {

            readingAddress = new IPAddress(new byte[] { 127, 0, 0, 1 });
            readingPort = 5555;
            clientIpEndPoint = new IPEndPoint(readingAddress, readingPort);
            clientSocket = new Socket(
                AddressFamily.InterNetwork,  // AddressFamily.InterNetworkV6
                SocketType.Dgram, 
                ProtocolType.Udp
            );
            clientSocket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
            //clientSocket.Bind(clientIpEndPoint);
            clientSocket.ReceiveTimeout = 100;
            clientSocket.Connect(clientIpEndPoint);


            log_message("Client connected: " + clientSocket.Connected);
            // SocketType
            // Stream = 1,
            // Dgram = 2,
            // Raw = 3,
            // Rdm = 4,
            // Seqpacket = 5

            return true; }
        // public override void start_experiment() {}
        // public override void start_routine() {}
        public override void update() {

            int count = clientSocket.Receive(buffer);
            if (count > 0) {
                string message = Encoding.UTF8.GetString(buffer, 0, count);
                log_message("message: " + message);
            } else {
                log_message("timeout");
            }
        }
        // public override void stop_routine() {}
        // public override void stop_experiment(){}
        // public override void play(){}
        // public override void pause(){}
        // public override void set_update_state(bool doUpdate) { }        
        // public override void set_visibility(bool visibility) { }

        // # for advanced users 
        // public override void clean(){}
        // public override void pre_update() {}
        // public override void post_update() {}
        // public override void update_parameter_from_gui(XML.Arg arg){}
        // public override void update_from_current_config(){}

        // # slots
        // public override void slot1(object value){}
        // public override void slot2(object value){}
        // public override void slot3(object value){}
        // public override void slot4(object value){}        
        // public override void slot5(IdAny idValue){
        // 	int id = idValue.id;
        // 	object value = idValue.value;
        // }  
    }
}


/**
namespace Ex{    

    public class BaseCompiledCSharpComponent : MonoBehaviour{

        // ExComponent class associated to the script (see ExComponent code snippet) 
        public ExComponent p = null;
        
        // init configuration for this component 
        public ComponentInitConfig init_config();
        // current configuration of the routine for this component
        public ComponentConfig current_config();
        // current routine associated to this component
        public Routine current_routine();
        // current condition of the associated routine
        public Condition current_condition();
        
        // states
        public bool is_initialized();
        public bool is_visible();
        public bool is_updating();
        public bool is_closed();

        // getters of others components
        public List<ExComponent> get_all(string category);
        public List<ExComponent> get_all(ExComponent.CType type);
        public ExComponent get(int key);
        public ExComponent get(string name);
        public List<T> get<T>() where T : ExComponent;
        public T get<T>(string name) where T : ExComponent;

        // special commands
        public void next();
        public void previous();
        public void close();
        public void stop();
        
        // timers
        public double ellapsed_time_exp_ms();
        public double ellapsed_time_routine_ms();
        
        // logging
        public void log_message(string message);
		public void log_warning(string warning);
        public void log_error(string error);
        
        // invoke signals
        public void invoke_signal1(object value);
        public void invoke_signal2(object value);
        public void invoke_signal3(object value);
        public void invoke_signal4(object value);
        
    }
		
	public class ExComponent : MonoBehaviour{

		// enums
		public enum CType { UI, Image, Video, Tracking, Model, Avatar, Audio, Script, Scene, Network, Output, Input, Text, Interaction, Feedback, Camera, Settings, Cloud, Undefined };
		public enum InitPriority { Low, Medium, Hight};

		// members
		// # misc
        public int key;
        public CType type;
        public InitPriority initPriority;

		// # data 
		public Routine currentRoutine;
		public Condition currentCondition;
        public TimeLine currentTimeline;      
        // ## configs
        public ComponentConfig currentC; // current config
        public ComponentInitConfig initC; // init config
        public List<ComponentConfig> configs;		

        // # events
        public Events.SignalsSlotsConnections events();

		// functions		
		// # misc
		public string component_name();
		public string type_str();               
        
		// # log
        static public void log_message(string message);
        static public void log_error(string error);
		
		// # time
        static public double ellapsed_time_exp_ms();
        static public double ellapsed_time_routine_ms();
		
		// # get
        static public List<ExComponent> get_all(string category);
        static public List<ExComponent> get_all(CType type);
        static public ExComponent get(int key);
        static public ExComponent get(string name);
        static public List<T> get<T>() where T : ExComponent;
        static public T get<T>(string name) where T : ExComponent;

		// # specials commands
        public static void next(); //  Send an event for moving to the next flow element        
        public static void previous(); // Send an event for moving to the previous flow element
        public static void stop();
        public static void next_element_with_name(string elementName);
        public static void previous_element_with_name(string elementName);
        public static void next_element_with_condition(string elementName);
        public static void previous_element_with_condition(string elementName);
        public static void modify_routine_action_config(string routineName, string conditionName, string componentName, string newConfigName);
		public void close(); // Disable the component, it will be not enabled again before the next routine
		
		// # configs
        public ComponentInitConfig init_config();
        public ComponentConfig current_config();
        public ComponentConfig get_config(string configName);

		// # states
        public bool is_started();
        public bool is_initialized();
        public bool is_visible();
        public bool is_updating();
        public bool is_closed();
    }
}
*/
