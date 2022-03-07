
/***********************************************************************************
** exvr-exp                                                                       **
** MIT License                                                                    **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                                **
** Permission is hereby granted, free of charge, to any person obtaining a copy   **
** of this software and associated documentation files (the "Software"), to deal  **
** in the Software without restriction, including without limitation the rights   **
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      **
** copies of the Software, and to permit persons to whom the Software is          **
** furnished to do so, subject to the following conditions:                       **
**                                                                                **
** The above copyright notice and this permission notice shall be included in all **
** copies or substantial portions of the Software.                                **
**                                                                                **
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     **
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       **
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    **
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         **
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  **
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  **
** SOFTWARE.                                                                      **
************************************************************************************/

// system
using System;
using System.Collections.Generic;

namespace Ex{

    [Serializable]
    public class TimeLine{

        public TimeLine(XML.Timeline updateTimeline, XML.Timeline visiblityTimeline) {

            // generate intervals
            for (int ii = 0; ii < updateTimeline.Interval.Count; ++ii) {
                updateIntervals.Add(new Interval(updateTimeline.Interval[ii].T1, updateTimeline.Interval[ii].T2));
            }
            for (int ii = 0; ii < visiblityTimeline.Interval.Count; ++ii) {
                visiblityIntervals.Add(new Interval(visiblityTimeline.Interval[ii].T1, visiblityTimeline.Interval[ii].T2));
            }
        }

        public bool is_timeLine_enabled(double timeS) {

            bool enabled = false;
            foreach (Interval interval in updateIntervals) {
                enabled |= interval.is_in_interval(timeS);
            }
            return enabled;
        }

        public double max_time_update_s() {
            double max = 0;
            foreach (Interval interval in updateIntervals) {
                if (interval.tEndS > max) {
                    max = interval.tEndS;
                }
            }
            return max;
        }

        public double max_time_visibility_s() {
            double max = 0;
            foreach (Interval interval in visiblityIntervals) {
                if (interval.tEndS > max) {
                    max = interval.tEndS;
                }
            }
            return max;
        }

        public double max_time_s() {
            return Math.Max(max_time_update_s(), max_time_visibility_s());
        }


        public bool inside_update_intervals(double timeS) {
            
            foreach (Interval interval in updateIntervals) {
                if (interval.is_in_interval(timeS)) {
                    return true;
                }
            }
            return false;
        }

        public bool inside_visibility_intervals(double timeS) {

            foreach (Interval interval in visiblityIntervals) {
                if (interval.is_in_interval(timeS)) {
                    return true;
                }
            }
            return false;
        }

        public Interval get_upate_time_interval(double timeS){
            foreach (Interval interval in updateIntervals) {
                if (interval.is_in_interval(timeS)) {
                    return interval;
                }
            }
            return null;
        }

        public Interval get_visibility_time_interval(double timeS) {
            foreach (Interval interval in visiblityIntervals) {
                if (interval.is_in_interval(timeS)) {
                    return interval;
                }
            }
            return null;
        }

        public List<Interval> updateIntervals    = new List<Interval>();
        public List<Interval> visiblityIntervals = new List<Interval>();
    }
}