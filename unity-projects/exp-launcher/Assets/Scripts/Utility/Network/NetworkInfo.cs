
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
using System.Net;
using System.Net.Sockets;
using System.Collections.Generic;

namespace Ex {

    public class NetworkInfo {

        public static IPAddress str_to_ip_address(string address, bool ipv6) {

            IPAddress ip;
            if (IPAddress.TryParse(address, out ip)) {
                if (ipv6) {
                    if (ip.AddressFamily == System.Net.Sockets.AddressFamily.InterNetwork) {
                        UnityEngine.Debug.LogError("Address " + address + " is not ipv6");
                        return null;
                    }
                } else {
                    if (ip.AddressFamily == System.Net.Sockets.AddressFamily.InterNetworkV6) {
                        UnityEngine.Debug.LogError("Address " + address + " is not ipv4");
                        return null;
                    }
                }

                return ip;
            }

            return null;
        }

        public static List<IPAddress> get_ip_addresses_from_hostname(string hostName, bool ipv6) {

            try {
                var addresses = (System.Net.Dns.GetHostAddresses(hostName));

                List<IPAddress> addressesList = new List<IPAddress>();
                foreach (var address in addresses) {

                    if (address.AddressFamily == System.Net.Sockets.AddressFamily.InterNetwork) { // ipv4
                        if (ipv6) {
                            continue;
                        }
                    }

                    if (address.AddressFamily == System.Net.Sockets.AddressFamily.InterNetworkV6) { // ipv6
                        if (!ipv6) {
                            continue;
                        }
                    }

                    if (address.IsIPv6Multicast) {
                        // A multicast address is a logical identifier for a group of hosts in a computer network,
                        //that are available to process datagrams or frames intended to be multicast for a designated network service.
                        // Multicast addressing can be used in the Link Layer (Layer 2 in the OSI model), such as Ethernet multicast,
                        // and at the Internet Layer (Layer 3 for OSI) for Internet Protocol Version 4 (IPv4) or Version 6 (IPv6) multicast.
                        continue;
                    }
                    if (address.IsIPv6LinkLocal) {
                        // A link-local address is an IP address that is intended only for communications within the local 
                        // subnetwork.Routers do not forward packets with link-local addresses.
                        continue;
                    }
                    //if (ad.IsIPv4MappedToIPv6) {
                    //    // Dual-stack sockets always require IPv6 addresses. The ability to interact with an IPv4 address requires the use of the IPv4-mapped
                    //    // IPv6 address format. Any IPv4 addresses must be represented in the IPv4-mapped IPv6 address format which enables an IPv6 only
                    //    // application to communicate with an IPv4 node. The IPv4-mapped IPv6 address format allows the IPv4 address of an IPv4 node to be 
                    //    // represented as an IPv6 address. The IPv4 address is encoded into the low-order 32 bits of the IPv6 address, and the high-order 96
                    //    // bits hold the fixed prefix 0:0:0:0:0:FFFF. The IPv4-mapped IPv6 address format is specified in RFC 4291. For more information, see 
                    //    // www.ietf.org/rfc/rfc4291.txt.
                    //    continue;
                    //}
                    //if (ad.IsIPv6SiteLocal) {
                    //    // A unique local address(ULA) is an IPv6 address in the block fc00::/ 7, defined in RFC 4193.It is the IPv6 counterpart 
                    //    // of the IPv4 private address.Unique local addresses are available for use in private networks, e.g.inside a single site
                    //    // or organisation, or spanning a limited number of sites or organisations.They are not routable in the global IPv6 Internet.
                    //    continue;
                    //}

                    //if (ad.IsIPv6Teredo) {
                    //    // In computer networking, Teredo is a transition technology that gives full IPv6 connectivity for IPv6 - capable hosts which are
                    //    // on the IPv4 Internet but which have no direct native connection to an IPv6 network.
                    //    // Compared to other similar protocols its distinguishing feature is that it is able to perform its function even 
                    //    // from behind network address translation(NAT) devices such as home routers.
                    //    continue;
                    //}
                    addressesList.Add(address);
                }

                if (addressesList.Count == 0) {
                    UnityEngine.Debug.LogError("No ip addresses could be found from host " + hostName);
                }

                return addressesList;
            } catch (SocketException ex) {
                UnityEngine.Debug.LogError(ex.Message); // An error is encountered when resolving hostNameOrAddress.
            } catch (ArgumentOutOfRangeException ex) {
                UnityEngine.Debug.LogError(ex.Message); // The length of hostNameOrAddress is greater than 255 characters.
            } catch (ArgumentException ex) {
                UnityEngine.Debug.LogError(ex.Message); // hostName is an invalid IP address.
            }

            return null;
        }

        public static List<IPAddress> get_ip_addresses(string address, bool ipv6 = false) {

            List<IPAddress> ipAddresses = new List<IPAddress>();

            var ipAddress = str_to_ip_address(address, ipv6);
            if (ipAddress != null) {
                ipAddresses.Add(ipAddress);
                return ipAddresses;
            }

            ipAddresses = get_ip_addresses_from_hostname(address, ipv6);
            if (ipAddresses != null) {
                return ipAddresses;
            }

            return new List<IPAddress>();
        }
    }
}