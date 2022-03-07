﻿//
// Copyright 2017 Valve Corporation. All rights reserved. Subject to the following license:
// https://valvesoftware.github.io/steam-audio/license.html
//

using UnityEngine;

namespace SteamAudio
{
    public class SteamAudioStaticMesh : MonoBehaviour
    {
        [Header("Export Settings")]
        public SerializedData asset = null;
        public string sceneNameWhenExported = "";

        StaticMesh mStaticMesh = null;

        void Start()
        {
            if (asset == null)
            {
                Debug.LogWarningFormat("No asset set for Steam Audio Static Mesh in scene {0}. Export the scene before clicking Play.",
                    gameObject.scene.name);
            }
        }

        void OnEnable()
        {
            if (mStaticMesh != null)
            {
                mStaticMesh.AddToScene(SteamAudioManager.CurrentScene);
            }
        }

        void OnDisable()
        {
            if (mStaticMesh != null && SteamAudioManager.CurrentScene != null)
            {
                mStaticMesh.RemoveFromScene(SteamAudioManager.CurrentScene);
            }
        }

        void Update()
        {
            if (mStaticMesh == null && asset != null)
            {
                mStaticMesh = new StaticMesh(SteamAudioManager.Context, SteamAudioManager.CurrentScene, asset);

                if (enabled)
                {
                    mStaticMesh.AddToScene(SteamAudioManager.CurrentScene);
                }
            }
        }
    }
}
