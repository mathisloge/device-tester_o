#pragma once
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <chrono>
#include <iostream>
#include <string>
#include "../nmea_messages.hpp"

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;



BOOST_FUSION_ADAPT_STRUCT(Time, (uint32_t, hours)(uint32_t, minutes)(uint32_t, seconds)(uint32_t, milliseconds))
// clang-format off
BOOST_FUSION_ADAPT_STRUCT(nmea::detail::NmeaLatLonRep,
                        (uint32_t, degrees)
                        (double, minutes)
                        (DirectionIndicator, indicator))
// clang-format on

namespace nmea::detail
{

    struct TalkerSymbol : qi::symbols<char, Talker>
    {
        TalkerSymbol()
        {
            this->add("GP", Talker::GP)("GL", Talker::GL)("GA", Talker::GA)("GB", Talker::GB)("GN", Talker::GN);
        }
    };
    struct DatumRefSymbol : qi::symbols<char, DatumRef>
    {
        DatumRefSymbol()
        {
            this->add("W84", DatumRef::wgs84)("P90", DatumRef::pz90)("999", DatumRef::user_defined);
        }
    };

    struct NSDirSymbol : qi::symbols<char, DirectionIndicator>
    {
        NSDirSymbol()
        {
            this->add("N", DirectionIndicator::north)("S", DirectionIndicator::south);
        }
    };
    struct EWDirSymbol : qi::symbols<char, DirectionIndicator>
    {
        EWDirSymbol()
        {
            this->add("E", DirectionIndicator::east)("W", DirectionIndicator::west);
        }
    };

    static const TalkerSymbol kTalkerSymbol;
    static const DatumRefSymbol kDatumRefSymbol;
    static const NSDirSymbol kNSSymbol;
    static const EWDirSymbol kEWSymbol;


    template <typename Iterator, typename DirSymbol, int DegreeLen>
    struct lat_lon_grammar : qi::grammar<Iterator, NmeaLatLonRep()>
    {
        lat_lon_grammar() : lat_lon_grammar::base_type(start_)
        {
            using qi::double_;
            start_ = deg_parser_ >> double_ >> ',' >> dir_parser_;
        };
        DirSymbol dir_parser_;
        qi::uint_parser<uint32_t, 10, DegreeLen, DegreeLen> deg_parser_;
        qi::rule<Iterator, NmeaLatLonRep()> start_;
    };

    template <typename Iterator>
    struct time_grammar : qi::grammar<Iterator, Time()>
    {
        time_grammar() : time_grammar::base_type(start_)
        {
            static const qi::uint_parser<uint32_t, 10, 2, 2> time_parser{};

            start_ = time_parser >> time_parser >> time_parser >> '.' >> time_parser;
        };
        qi::rule<Iterator, Time()> start_;
    };
} // namespace nmea::detail