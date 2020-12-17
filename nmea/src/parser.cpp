#include "grammer.hpp"

int main()
{
    std::string stro = "$\r\n$GPRMC,162614,A,5230.5900,N,01322.3900,E,10.0,90.0,131006,1.2,E,A*13\r\n$GPRMC,HHMMSS,A,BBBB.BBBB,b,LLLLL.LLLL,l,GG.G,RR.R,DDMMYY,M.M,m,F*PP\r\n";
    std::string datum = "$GPDTM,999,,0.08,N,0.07,E,-47.7,W84*1B\r\n";
    std::string func = "$GPRMC\r\n";
    std::string str = datum;
    Nmea c;
    if (parse_nmea(str.begin(), str.end(), c))
    {
        std::cout << "Parsing succeeded" << std::endl;
        std::cout << "got message: " << std::endl;
    }
    else
    {
        std::cout << "Parsing failed" << std::endl;
    }
    return 0;
}