#include <vector>
#include <numeric>
#include "nmea/nmea_parser.hpp"
class Handler {
public:
    void handle(const NmeaMessage &msg)
    {
        std::cout << "got unkown nmea message " << std::endl;
    }
    void handle(const NmeaUnsupported &msg)
    {
        std::cout << "got dtm nmea message " << msg.message << std::endl;
    }
    void handle(const NmeaDTM &msg)
    {
        std::cout << "got dtm nmea message " << std::endl;
    }
    void handle(const NmeaGLL &msg)
    {
        std::cout << "got gll nmea message " << std::endl;
        std::cout << "lat: " << msg.latitude << " lon: " << msg.longitude << std::endl;
    }
};

int main()
{
    std::string stro = "$\r\n$GPRMC,162614,A,5230.5900,N,01322.3900,E,10.0,90.0,131006,1.2,E,A*13\r\n$GPRMC,HHMMSS,A,BBBB.BBBB,b,LLLLL.LLLL,l,GG.G,RR.R,DDMMYY,M.M,m,F*PP\r\n";
    std::string dtm = "$GPDTM,999,,0.08,N,0.07,E,-47.7,W84*1B\r\n";
    std::string gll{"as$GNGLL,5304.98825,N,00852.25036,E,221313.00,A,A*76\r\ndwa"};
    std::string unsp{"$GLGSV,3,2,09,79,17,167,,80,65,215,,81,33,049,,82,82,345,,3*71\r\n"};

    Handler handler;
    nmea::detail::AllNmeaMessages msg_var;

    std::vector<uint8_t> data;
    data.insert(data.begin(), unsp.begin(), unsp.end());
    auto it = data.begin();
    nmea::parseOneWithDispatch(it, data.end(), handler);
    std::cout << "processed " << std::distance(data.begin(), it) << std::endl;
    return 0;
}