
/*******************************************************************************
** exvr-export                                                                **
** No license (to be defined)                                                 **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                            **
********************************************************************************/


//#include "duo_tracking_export.hpp"

//// std
//#include <algorithm>

//duo::DuoTracking *create_duo_tracking(){
//    return new duo::DuoTracking();
//}

//void delete_duo_tracking(duo::DuoTracking *duoTracking){
//    delete duoTracking;
//}

//int reset_duo_tracking(duo::DuoTracking *duoTracking){
//    return duoTracking->reset() ? 1 : 0;
//}

//int size_cloud_duo_tracking(duo::DuoTracking *duoTracking){
//    return  duoTracking->acquisition()->frame.vertices.size()/3;
//}

//void lock_data_duo_tracking(duo::DuoTracking *duoTracking, int lock){
//    if(lock == 1){
//        duoTracking->acquisition()->frame.mutex.lock();
//    }else{
//        duoTracking->acquisition()->frame.mutex.unlock();
//    }
//}

//void copy_cloud_duo_tracking(duo::DuoTracking *duoTracking, float *vertices, float *colors){
//    std::move(duoTracking->acquisition()->frame.vertices.begin(), duoTracking->acquisition()->frame.vertices.end(), vertices);
//    std::move(duoTracking->acquisition()->frame.colors.begin(), duoTracking->acquisition()->frame.colors.end(), colors);
//}

//void set_exposure_duo_tracking(duo::DuoTracking *duoTracking,float value){
//    duoTracking->acquisition()->set_exposure(static_cast<double>(value));
//}

//void set_gain_duo_tracking(duo::DuoTracking *duoTracking,float value){
//    duoTracking->acquisition()->set_gain(static_cast<double>(value));
//}

//void set_led_duo_tracking(duo::DuoTracking *duoTracking,float value){
//    duoTracking->acquisition()->set_led(static_cast<double>(value));
//}

//void set_horizontal_flip_duo_tracking(duo::DuoTracking *duoTracking,int hFlip){
//    duoTracking->acquisition()->set_horizontal_flip(hFlip==1);
//}

//void set_vertical_flip_duo_tracking(duo::DuoTracking *duoTracking,int vFlip){
//    duoTracking->acquisition()->set_vertical_flip(vFlip==1);
//}

//void set_factor_color_duo_tracking(duo::DuoTracking *duoTracking,float factorColor){
//    duoTracking->acquisition()->frame.factorColor = factorColor;
//}

//void set_min_color_duo_tracking(duo::DuoTracking *duoTracking,int minColor){
//    duoTracking->acquisition()->frame.minColor = minColor;
//}

//void set_max_color_duo_tracking(duo::DuoTracking *duoTracking,int maxColor){
//    duoTracking->acquisition()->frame.maxColor = maxColor;
//}

//void set_min_distance_duo_tracking(duo::DuoTracking *duoTracking, float minDistance){
//    duoTracking->acquisition()->frame.distMin = minDistance;
//}

//void set_max_distance_duo_tracking(duo::DuoTracking *duoTracking, float maxDistance){
//    duoTracking->acquisition()->frame.distMax = maxDistance;
//}

//void move_array_duo_tracking(int size, float *from, float *dest){
//    std::move(from, from + size, dest);
//}

//void set_dense_mode_duo_tracking(duo::DuoTracking *duoTracking, int mode){
//    duoTracking->acquisition()->params.mode = mode;
//    duoTracking->acquisition()->update_dense_params();
//}

//void set_dense_pre_filter_cap_duo_tracking(duo::DuoTracking *duoTracking, int preFilterCap){
//    duoTracking->acquisition()->params.preFilterCap = preFilterCap;
//    duoTracking->acquisition()->update_dense_params();
//}

//void set_dense_sad_window_size_duo_tracking(duo::DuoTracking *duoTracking, int sadWindowSize){
//    duoTracking->acquisition()->params.sadWindowSize = sadWindowSize;
//    duoTracking->acquisition()->update_dense_params();
//}

//void set_dense_uniqeness_ratio_duo_tracking(duo::DuoTracking *duoTracking, int uniqenessRatio){
//    duoTracking->acquisition()->params.uniqenessRatio = uniqenessRatio;
//    duoTracking->acquisition()->update_dense_params();
//}

//void set_dense_speckle_window_size_duo_tracking(duo::DuoTracking *duoTracking, int speckleWindowSize){
//    duoTracking->acquisition()->params.speckleWindowSize = speckleWindowSize;
//    duoTracking->acquisition()->update_dense_params();
//}

//void set_dense_speckle_range_duo_tracking(duo::DuoTracking *duoTracking, int speckleRange){
//    duoTracking->acquisition()->params.speckleRange = speckleRange;
//    duoTracking->acquisition()->update_dense_params();
//}
