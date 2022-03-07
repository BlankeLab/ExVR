
/*******************************************************************************
** exvr-export                                                                **
** No license (to be defined)                                                 **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                            **
********************************************************************************/

//#pragma once

//// components
//#include "duo3D/duo_tracking.hpp"

//// export
//#include "export_utility.hpp"

//extern "C"{

//    DECL_EXPORT duo::DuoTracking* create_duo_tracking();
//    DECL_EXPORT void delete_duo_tracking(duo::DuoTracking *duoTracking);
//    DECL_EXPORT int reset_duo_tracking(duo::DuoTracking *duoTracking);
//    DECL_EXPORT int size_cloud_duo_tracking(duo::DuoTracking *duoTracking);

//    DECL_EXPORT void copy_cloud_duo_tracking(duo::DuoTracking *duoTracking, float *vertices, float *colors);
//    DECL_EXPORT void move_array_duo_tracking(int size, float *from, float *dest);

//    DECL_EXPORT void lock_data_duo_tracking(duo::DuoTracking *duoTracking, int lock);

//    DECL_EXPORT void set_exposure_duo_tracking(duo::DuoTracking *duoTracking,float value);
//    DECL_EXPORT void set_gain_duo_tracking(duo::DuoTracking *duoTracking,float value);
//    DECL_EXPORT void set_led_duo_tracking(duo::DuoTracking *duoTracking,float value);
//    DECL_EXPORT void set_horizontal_flip_duo_tracking(duo::DuoTracking *duoTracking,int hFlip);
//    DECL_EXPORT void set_vertical_flip_duo_tracking(duo::DuoTracking *duoTracking,int vFlip);
//    DECL_EXPORT void set_factor_color_duo_tracking(duo::DuoTracking *duoTracking,float factorColor);
//    DECL_EXPORT void set_min_color_duo_tracking(duo::DuoTracking *duoTracking,int minColor);
//    DECL_EXPORT void set_max_color_duo_tracking(duo::DuoTracking *duoTracking,int maxColor);
//    DECL_EXPORT void set_min_distance_duo_tracking(duo::DuoTracking *duoTracking,float minDistance);
//    DECL_EXPORT void set_max_distance_duo_tracking(duo::DuoTracking *duoTracking,float maxDistance);

//    DECL_EXPORT void set_dense_mode_duo_tracking(duo::DuoTracking *duoTracking,int mode); // [0, 3] - [BM, SGBM, BM_HQ, SGBM_HQ]
//    DECL_EXPORT void set_dense_pre_filter_cap_duo_tracking(duo::DuoTracking *duoTracking,int preFilterCap); // [1, 63]
//    DECL_EXPORT void set_dense_sad_window_size_duo_tracking(duo::DuoTracking *duoTracking,int sadWindowSize);  // [2, 10]
//    DECL_EXPORT void set_dense_uniqeness_ratio_duo_tracking(duo::DuoTracking *duoTracking,int uniqenessRatio);  // [1, 10]
//    DECL_EXPORT void set_dense_speckle_window_size_duo_tracking(duo::DuoTracking *duoTracking,int speckleWindowSize);  // [0, 256]
//    DECL_EXPORT void set_dense_speckle_range_duo_tracking(duo::DuoTracking *duoTracking,int speckleRange);  // [0, 32]
////    uint32_t scale;                     // [0, 3] - [No Scale, Scale X, Scale Y, Scale X&Y]
//}
