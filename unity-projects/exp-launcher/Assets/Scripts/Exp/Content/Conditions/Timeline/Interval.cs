
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


namespace Ex{

    [Serializable]
    public class Interval{

        public Interval(double startS, double endS) {
            tStartS  = startS;
            tEndS    = endS;
        }

        public bool is_in_interval(double time) {
            return (time >= tStartS) && (time <= tEndS);
        }

        public double duration() {
            return tEndS - tStartS;
        }

        public double tStartS    = 0.0;
        public double tEndS      = 0.0;
    }

    [Serializable]
    public class TimestampInterval {

        public TimestampInterval(long startTick, long endTick) {
            this.startTick = startTick;
            this.endTick   = endTick;
        }

        public bool is_in_interval(long tick) {
            return (tick >= startTick) && (tick <= endTick);
        }

        public long startTick   = -1;
        public long endTick     = -1;
    }
}