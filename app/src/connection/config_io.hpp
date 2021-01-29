#pragma once
#include <memory>
#include <nlohmann/json.hpp>
#include "connection.hpp"
#include "serial_connection.hpp"
#include "tcp_connection.hpp"
#include "udp_connection.hpp"

// clang-format off
// partial specialization (full specialization works too)
namespace nlohmann {
    template <>
    struct adl_serializer<boost::asio::serial_port_base::parity> {
        static void to_json(json& j, const boost::asio::serial_port_base::parity& opt) {
            j = opt.value();
        }

        static void from_json(const json& j,boost::asio::serial_port_base::parity& opt) {
            if (j.is_null()) {
                opt = boost::asio::serial_port_base::parity(connection::SerialOptions::kDefaultPar);
            } else {
                opt = boost::asio::serial_port_base::parity(j.get<boost::asio::serial_port_base::parity::type>());
            }
        }
    };
}
namespace nlohmann {
    template <>
    struct adl_serializer<boost::asio::serial_port_base::character_size> {
        static void to_json(json& j, const boost::asio::serial_port_base::character_size& opt) {
           j = opt.value();
        }

        static void from_json(const json& j,boost::asio::serial_port_base::character_size& opt) {
            if (j.is_null()) {
                opt = boost::asio::serial_port_base::character_size(connection::SerialOptions::kDefaultCharSize);
            } else {
                opt = boost::asio::serial_port_base::character_size(j.get<unsigned int>());
            }
        }
    };
}
namespace nlohmann {
    template <>
    struct adl_serializer<boost::asio::serial_port_base::flow_control> {
        static void to_json(json& j, const boost::asio::serial_port_base::flow_control& opt) {
            j = opt.value();
        }

        static void from_json(const json& j,boost::asio::serial_port_base::flow_control& opt) {
            if (j.is_null()) {
                opt = boost::asio::serial_port_base::flow_control(connection::SerialOptions::kDefaultFlow);
            } else {
                opt = boost::asio::serial_port_base::flow_control(j.get<boost::asio::serial_port_base::flow_control::type>());
            }
        }
    };
}
namespace nlohmann {
    template <>
    struct adl_serializer<boost::asio::serial_port_base::stop_bits> {
        static void to_json(json& j, const boost::asio::serial_port_base::stop_bits& opt) {
             j = opt.value();
        }

        static void from_json(const json& j,boost::asio::serial_port_base::stop_bits& opt) {
            if (j.is_null()) {
                opt = boost::asio::serial_port_base::stop_bits(connection::SerialOptions::kDefaultStopBits);
            } else {
                opt = boost::asio::serial_port_base::stop_bits(j.get<boost::asio::serial_port_base::stop_bits::type>());
            }
        }
    };
}
// clang-format on

namespace connection
{
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SerialOptions, identifier, port, baud_rate, parity, char_size, flow_control, stop_bits)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TcpOptions, identifier, server, server_port, service, packet_end)
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(UdpOptions, identifier, write_address, write_port, listen_protocol, listen_port)
} // namespace connection
