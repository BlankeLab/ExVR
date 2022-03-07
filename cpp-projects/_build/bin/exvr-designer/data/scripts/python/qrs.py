
#from utility import * # check utility.py for functions documentation
#from component import *

#A = np.arange(10)
#print('input list = ',A)

import ex
import sys
import numpy as np
from collections import deque
#from time import gmtime, strftime
from scipy.signal import butter, lfilter

class Component(object):

    def initialize(self):        
        #ex.log("--From python: initialize --")        
        return True

    def start_experiment(self):
        #ex.log("From python: start_experiment ")
        pass

    def stop_experiment(self):
        #ex.log("From python: stop_experiment ")
        pass

    def start_routine(self):
        #ex.log("From python: start_routine ")
        pass

    def stop_routine(self):
        #ex.log("From python: stop_routine ")
        pass

    def update(self):       
        pass

    def update_parameter_from_gui(self):
        pass        
        
    def stop(self):
        pass

    def play(self):
        pass

    def pause(self):
        pass

    def set_visibility(self, visible):
        #ex.log("From python: set_visibility " + str(visible))
        pass

    def set_update_state(self, doUpdate):
        #ex.log("From python: set_update_state " + str(doUpdate))
        pass

    def clean(self):
        #ex.log("From python: clean ")
        pass

    def slot(self, indexPort, value):
        pass

class DefaultComponent(Component):
    


    
    def detect_qrs(self, detected_peaks_values):
        """
        Method responsible for classifying detected ECG measurements peaks either as noise or as QRS complex (heart beat).
        :param array detected_peaks_values: detected peaks values array
        """
        self.samples_since_last_detected_qrs += 1

        # After a valid QRS complex detection, there is a 200 ms refractory period before next one can be detected.
        if self.samples_since_last_detected_qrs > self.refractory_period:

            # Check whether any peak was detected in analysed samples window.
            if len(detected_peaks_values) > 0:

                # Take the last one detected in analysed samples window as the most recent.
                most_recent_peak_value = detected_peaks_values[-1]

                # Peak must be classified either as a noise peak or a QRS peak.
                # To be classified as a QRS peak it must exceed dynamically set threshold value.
                if most_recent_peak_value > self.threshold_value:
                    self.handle_detection()
                    self.samples_since_last_detected_qrs = 0

                    # We mark QRS detection with '1' flag in 'qrs_detected' log column ('0' otherwise).
                    self.detected_qrs = 1

                    # Adjust QRS peak value used later for setting QRS-noise threshold.
                    self.qrs_peak_value = self.qrs_peak_filtering_factor * most_recent_peak_value + \
                                             (1 - self.qrs_peak_filtering_factor) * self.qrs_peak_value
                else:
                    # Adjust noise peak value used later for setting QRS-noise threshold.
                    self.noise_peak_value = self.noise_peak_filtering_factor * most_recent_peak_value + \
                                            (1 - self.noise_peak_filtering_factor) * self.noise_peak_value

                # Adjust QRS-noise threshold value based on previously detected QRS or noise peaks value.
                self.threshold_value = self.noise_peak_value + \
                                       self.qrs_noise_diff_weight * (self.qrs_peak_value - self.noise_peak_value)		


    def handle_detection(self):
        """
        Method responsible for generating any kind of response for detected QRS complex.
        """
        #print("Pulse")
        pass

    def findpeaks(self, data, spacing=1, limit=None):
        """
        Janko Slavic peak detection algorithm and implementation.
        https://github.com/jankoslavic/py-tools/tree/master/findpeaks
        Finds peaks in `data` which are of `spacing` width and >=`limit`.
        :param ndarray data: data
        :param float spacing: minimum spacing to the next peak (should be 1 or more)
        :param float limit: peaks should have value greater or equal
        :return array: detected peaks indexes array
        """
        len = data.size
        x = np.zeros(len + 2 * spacing)
        x[:spacing] = data[0] - 1.e-6
        x[-spacing:] = data[-1] - 1.e-6
        x[spacing:spacing + len] = data
        peak_candidate = np.zeros(len)
        peak_candidate[:] = True
        for s in range(spacing):
            start = spacing - s - 1
            h_b = x[start: start + len]  # before
            start = spacing
            h_c = x[start: start + len]  # central
            start = spacing + s + 1
            h_a = x[start: start + len]  # after
            peak_candidate = np.logical_and(peak_candidate, np.logical_and(h_c > h_b, h_c > h_a))

        ind = np.argwhere(peak_candidate)
        ind = ind.reshape(ind.size)
        if limit is not None:
            ind = ind[data[ind] > limit]
        return ind    

    def bandpass_filter(self, data, lowcut, highcut, signal_freq, filter_order):
        """
        Method responsible for creating and applying Butterworth filter.
        :param deque data: raw data
        :param float lowcut: filter lowcut frequency value
        :param float highcut: filter highcut frequency value
        :param int signal_freq: signal frequency in samples per second (Hz)
        :param int filter_order: filter order
        :return array: filtered data
        """
        """Constructs signal filter and uses it to given data set."""
        nyquist_freq = 0.5 * signal_freq
        low = lowcut / nyquist_freq
        high = highcut / nyquist_freq
        b, a = butter(filter_order, [low, high], btype="band")
        y = lfilter(b, a, data)

        return y
    

    def detect_peaks(self, most_recent_measurements):
        """
        Method responsible for extracting peaks from recently received ECG measurements through processing.
        :param deque most_recent_measurements: most recent ECG measurements array
        """
        # Measurements filtering - 0-15 Hz band pass filter.
        filtered_ecg_measurements = self.bandpass_filter(most_recent_measurements, lowcut=self.filter_lowcut,
                                                         highcut=self.filter_highcut, signal_freq=self.signal_frequency,
                                                         filter_order=self.filter_order)

        # Derivative - provides QRS slope information.
        differentiated_ecg_measurements = np.ediff1d(filtered_ecg_measurements)

        # Squaring - intensifies values received in derivative.
        squared_ecg_measurements = differentiated_ecg_measurements ** 2

        # Moving-window integration.
        integrated_ecg_measurements = np.convolve(squared_ecg_measurements, np.ones(self.integration_window))

        # Fiducial mark - peak detection on integrated measurements.
        detected_peaks_indices = self.findpeaks(data=integrated_ecg_measurements,
                                                limit=self.findpeaks_limit,
                                                spacing=self.findpeaks_spacing)
        detected_peaks_indices = detected_peaks_indices[
            detected_peaks_indices > self.number_of_samples_stored - self.detection_window]
        detected_peaks_values = integrated_ecg_measurements[detected_peaks_indices]

        self.detect_qrs(detected_peaks_values=detected_peaks_values)    
    


    def initialize(self):      

        ex.log("From python: initialize ")

        # Configuration parameters.
        self.signal_frequency = 250  # Set ECG device frequency in samples per second here.

        self.number_of_samples_stored = 200  # Change proportionally when adjusting frequency (in samples).
        self.possible_measurement_upper_limit = 10  # ECG device physiologically upper measurement limit.
        
        self.filter_lowcut = 0.1
        self.filter_highcut = 15.0
        self.filter_order = 1
        
        self.integration_window = 15  # Change proportionally when adjusting frequency (in samples).
        
        self.findpeaks_limit = 0.35
        self.findpeaks_spacing = 50  # Change proportionally when adjusting frequency (in samples).
        self.detection_window = 40  # Change proportionally when adjusting frequency (in samples).
        
        self.refractory_period = 120  # Change proportionally when adjusting frequency (in samples).
        self.qrs_peak_filtering_factor = 0.125
        self.noise_peak_filtering_factor = 0.125
        self.qrs_noise_diff_weight = 0.25
        
        # Measurements and calculated values.
        self.timestamp = 0
        self.measurement = 0
        self.detected_qrs = 0
        self.most_recent_measurements = deque([0], self.number_of_samples_stored)
        self.samples_since_last_detected_qrs = 0
        self.qrs_peak_value = 0.0
        self.noise_peak_value = 0.0
        self.threshold_value = 0.0
        ex.log("From python: initialize " + str(self.threshold_value))         	
                    
        return True
    
    def start_routine(self):        
        ex.log("From python: start_routine " + str(self.signal_frequency) + " " + str(self.key))        
        pass    
    
    def update(self):           
        #ex.log(str(ex.ellapsed_time_since_exp()))
        pass


    def slot(self, indexPort, value):
    
        #ex.log("--start slot " + str(indexPort) + " " + str(self.key))# + " " + str(len(value)))        
        if indexPort == 0: # array of float
            for val in value:
                self.most_recent_measurements.append(val)            

        if indexPort == 1: # single float value
            self.most_recent_measurements.append(value)
                     
        
        self.detect_peaks(self.most_recent_measurements)
        ex.signal_int(self.key, 0, self.samples_since_last_detected_qrs)
        ex.signal_double(self.key, 1, self.qrs_peak_value)
        #○ex.log("Peak " + str(self.samples_since_last_detected_qrs))
        
        pass

