#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <iostream>
#include <string>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

struct Nmea
{
};

template <typename Iterator>
struct talker_grammar : qi::grammar<Iterator>
{
    talker_grammar() : talker_grammar::base_type(complete)
    {
        using boost::spirit::ascii::string;
        //! \todo save talker
        complete = string("GP") | string("GL") | string("GA") | string("GB") | string("GN");
    };
    qi::rule<Iterator> complete;
};

template <typename Iterator>
struct sentence_dtm_grammar : qi::grammar<Iterator>
{
    sentence_dtm_grammar() : sentence_dtm_grammar::base_type(complete)
    {
        using boost::spirit::ascii::string;
        using boost::spirit::qi::char_;
        using phoenix::ref;
        using qi::_1;
        using qi::double_;

        start = string("DTM");
        //! \todo save decoder
        datum = string("W84") | string("P90") | string("999");
        complete = start >> ',' >>
                   datum >> ',' >>
                   /*sub_datum*/ ',' >>
                   double_[ref(offset_lat_min) = _1] >> ',' >>
                   char_('N', 'S')[ref(offset_lat_indicator)] >> ',' >>
                   double_[ref(offset_lon_min) = _1] >> ',' >>
                   char_('E', 'W')[ref(offset_lon_indicator)] >> ',' >>
                   double_[ref(offset_altitude_m) = _1] >> ',' >>
                   string("W84");
    };
    qi::rule<Iterator> start;
    qi::rule<Iterator> datum;
    qi::rule<Iterator> complete;

    double offset_lat_min;
    char offset_lat_indicator;
    double offset_lon_min;
    char offset_lon_indicator;
    double offset_altitude_m;
};

template <typename Iterator>
uint8_t nmea0183_checksum(Iterator first, Iterator last)
{
    first++;   // skip $
    last -= 5; // skip *cc\r\n
    uint8_t crc = 0;
    while (first != last)
        crc ^= *first++;
    return crc;
}

template <typename Iterator>
bool parse_nmea(Iterator first, Iterator last, Nmea &c)
{
    Iterator mem_first = first;
    Iterator mem_last = last;
    talker_grammar<Iterator> talker;
    sentence_dtm_grammar<Iterator> sentence_dtm;
    using ascii::space;
    using phoenix::ref;
    using qi::_1;
    using qi::char_;
    using qi::double_;
    using qi::phrase_parse;
    using qi::uint_parser;

    double rN = 0.0;
    double iN = 0.0;
    uint8_t checksum = 0;
    bool r = phrase_parse(first, last,

                          //  Begin grammar
                          ('$' >> talker >> sentence_dtm >> '*' >> uint_parser<uint8_t, 16, 1, 2>()[ref(checksum) = _1]),
                          //  End grammar

                          char_("\r\n"));
    if (r)
    {
        const auto calc_check = nmea0183_checksum(mem_first, first);
        if (calc_check != checksum)
        {
            std::cout << "checksum mismatch: " << (calc_check == checksum) << " calc: " << static_cast<int>(calc_check) << " got " << static_cast<int>(checksum) << std::endl;
            return false;
        }
    }
    c = Nmea{};
    return r;
}
