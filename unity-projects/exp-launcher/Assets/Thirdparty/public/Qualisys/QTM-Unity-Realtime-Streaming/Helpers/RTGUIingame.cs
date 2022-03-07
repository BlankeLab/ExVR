﻿// Unity SDK for Qualisys Track Manager. Copyright 2015 Qualisys AB
//
using UnityEngine;
using System.Collections;
using QTMRealTimeSDK;
using System.Collections.Generic;
using System.Linq;
using QTMRealTimeSDK.Data;

namespace QualisysRealTime.Unity
{

    public class RTGUIingame : MonoBehaviour
    {
        short portUDP = 4545;
        int selectedServer = 0;
        DiscoveryResponse? selectedDiscoveryResponse = null;

        string connectionStatus = "Not Connected";

        bool connected = false;

        List<DiscoveryResponse> discoveryResponses;

        /// This makes sure we only can connect when in playing mode
        void OnInspectorUpdate()
        {
            if (!Application.isPlaying)
            {
                OnDisconnect();
                connected = false;
            }
        }

        void OnGUI()
        {
            GUIStyle style = new GUIStyle();
            style.fontStyle = FontStyle.Bold;
            style.normal.textColor = Color.white;
            GUI.Box(new Rect(10, 10, 220, 155), "Qualisys Realtime Streamer");

            GUI.Label(new Rect(20, 40, 200, 40), "QTM Server:\n(switch with arrow keys)");

            if (discoveryResponses == null) discoveryResponses = RTClient.GetInstance().GetServers();

            List<GUIContent> serverSelection = new List<GUIContent>();
            foreach (var discoveryResponse in discoveryResponses)
            {
                serverSelection.Add(new GUIContent(discoveryResponse.HostName + " (" + discoveryResponse.IpAddress + ":" + discoveryResponse.Port + ") " + discoveryResponse.InfoText));
            }

            GUI.Label(new Rect(20, 75, 200, 40), serverSelection[selectedServer], style);

            if (Input.GetKeyDown(KeyCode.LeftArrow) && !connected)
            {
                selectedServer--;
                if (selectedServer < 0)
                {
                    selectedServer += serverSelection.Count;
                }
            }
            else if (Input.GetKeyDown(KeyCode.RightArrow) && !connected)
            {
                selectedServer++;
                if (selectedServer > serverSelection.Count - 1)
                {
                    selectedServer = 0;
                }
            }
            selectedDiscoveryResponse = discoveryResponses[selectedServer];

            if (connected)
            {
                if (GUI.Button(new Rect(20, 115, 200, 40), "Disconnect"))
                {
                    OnDisconnect();
                }
            }
            else
            {
                if (GUI.Button(new Rect(20, 115, 200, 40), "Connect"))
                {
                    OnConnect();
                }
            }
            GUI.Label(new Rect(20, 90, 200, 40), "Status: " + connectionStatus);
        }

        void OnDestroy()
        {
            RTClient.GetInstance().Disconnect();
            connected = false;
        }

        void OnDisconnect()
        {
            RTClient.GetInstance().Disconnect();
            connected = false;

            connectionStatus = "Disconnected";
        }

        void OnConnect()
        {
            if (selectedDiscoveryResponse.HasValue)
                connected = RTClient.GetInstance().Connect(selectedDiscoveryResponse.Value, portUDP, true, true);

            if (connected)
                connectionStatus = "Connected";
            else
                connectionStatus = "Connection error - check console";
        }
    }
}