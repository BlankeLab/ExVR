﻿// Unity SDK for Qualisys Track Manager. Copyright 2015 Qualisys AB
//
#if UNITY_EDITOR
using UnityEngine;
using UnityEditor;
using System.Collections.Generic;
using QTMRealTimeSDK.Data;

namespace QualisysRealTime.Unity
{
    public class RTGUI : EditorWindow
    {
        short portUDP = 4545;
        DiscoveryResponse? selectedDiscoveryResponse = null;

        string connectionStatus = "Not Connected";

        bool connected = false;
        bool stream6d = true;
        bool stream3d = true;

        private List<SixDOFBody> availableBodies;
        private List<DiscoveryResponse> discoveryResponses;

        [MenuItem("Window/Qualisys/RTClient")]
        public static void ShowWindow()
        {
            EditorWindow.GetWindow(typeof(RTGUI));
        }

        public void OnEnable()
        {
            discoveryResponses = RTClient.GetInstance().GetServers();
        }

        /// This makes sure we only can connect when in playing mode
        void OnInspectorUpdate()
        {
            Repaint();
            if (!Application.isPlaying)
            {
                OnDisconnect();
                connected = false;
            }
        }

        private int serverNumber = 0;

        void OnGUI()
        {
            titleContent.text = "QTM Streaming";
            GUILayout.Label("Server Settings", EditorStyles.boldLabel);
            if (Application.isPlaying)
            {
                selectedDiscoveryResponse = null;
                if (discoveryResponses != null)
                {
                    GUILayout.Label("QTM Servers:");
                    List<GUIContent> serverSelection = new List<GUIContent>();
                    foreach (var discoveryResponse in discoveryResponses)
                    {
                        serverSelection.Add(new GUIContent(discoveryResponse.HostName + " (" + discoveryResponse.IpAddress + ":" + discoveryResponse.Port + ") " + discoveryResponse.InfoText));
                    }
                    serverNumber = EditorGUILayout.Popup(serverNumber, serverSelection.ToArray());
                    if (serverNumber >= 0 && serverNumber < discoveryResponses.Count)
                    {
                        selectedDiscoveryResponse = discoveryResponses[serverNumber];
                    }
                }
                else
                {
                    GUILayout.Label("No QTM Servers where discovered on the network");
                    return;
                }
            }
            else
            {
                GUILayout.Label("(Unity needs to be in play mode to set server)");
            }
            if (connected)
            {
                GUI.enabled = false;
            }
            GUILayout.Label("Stream Settings", EditorStyles.boldLabel);
            portUDP = (short)EditorGUILayout.IntField("UDP Port:", portUDP);
            stream3d = EditorGUILayout.Toggle("3D Labeled Markers", stream3d);
            stream6d = EditorGUILayout.Toggle("6DOF Objects", stream6d);
            GUI.enabled = true;

            if (Application.isPlaying)
            {
                GUILayout.Label("Status: " + connectionStatus);

                if (connected)
                {
                    if (GUILayout.Button("Disconnect"))
                    {
                        OnDisconnect();
                    }
                    GUILayout.Label("Available Bodies:");
                    if (availableBodies != null)
                    {
                        foreach (SixDOFBody body in availableBodies)
                        {
                            GUILayout.Label(body.Name);
                        }
                    }
                }
                else
                {
                    if (GUILayout.Button("Connect"))
                    {
                        OnConnect();
                    }
                }
            }
            else
            {
                GUILayout.Label("Please start Play to start streaming", EditorStyles.boldLabel);
            }
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
            {
                connected = RTClient.GetInstance().Connect(selectedDiscoveryResponse.Value, portUDP, stream6d, stream3d);
            }

            if (connected)
            {
                connectionStatus = "Connected";
                availableBodies = RTClient.GetInstance().Bodies;
            }
            else
            {
                connectionStatus = "Connection error - " + RTClient.GetInstance().GetErrorString() + " (also check console)";
            }
        }
    }
}
#endif