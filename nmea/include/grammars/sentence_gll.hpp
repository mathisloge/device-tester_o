#pragma once
#include "base_grammar.hpp"

// clang-format off
BOOST_FUSION_ADAPT_STRUCT(NmeaGLL,
                        (Talker, talker)
                        (double, latitude)
                        (double, longitude)
                        (Time, time)
                        (bool, data_valid)
                        (PositionFixStatus, position_fix))
// clang-format on
namespace nmea::detail
{

    struct GLLDataValidSymbol : qi::symbols<char, bool>
    {
        GLLDataValidSymbol()
        {
            this->add("V", false)("A", true);
        }
    };
    struct GLLPosisitionSymbol : qi::symbols<char, PositionFixStatus>
    {
        GLLPosisitionSymbol()
        {
            // clang-format off
            this->add("N", PositionFixStatus::no_fix)
                    ("E", PositionFixStatus::dead_reckoning)
                    ("A", PositionFixStatus::gnss_fix)
                    ("D", PositionFixStatus::differential_fix)
                    ("F", PositionFixStatus::rtk_float)
                    ("R", PositionFixStatus::rtk_fixed);
            // clang-format on
        }
    };

    template <typename Iterator>
    struct sentence_gll_grammar : public qi::grammar<Iterator, NmeaGLL()>
    {
        sentence_gll_grammar()
            : sentence_gll_grammar::base_type(sentence_)
        {
            using ascii::string;
            using qi::double_;
            static const GLLDataValidSymbol data_valid_sym;
            static const GLLPosisitionSymbol pos_sym;
            static const lat_lon_grammar<Iterator, NSDirSymbol, 2> lat_g_;
            static const lat_lon_grammar<Iterator, EWDirSymbol, 3> lon_g_;
            static const time_grammar<Iterator> time_g_;
            sentence_ = kTalkerSymbol >> "GLL" >> ',' >>
                        lat_g_ >> ',' >>
                        lon_g_ >> ',' >>
                        time_g_ >> ',' >>
                        data_valid_sym >> ',' >>
                        pos_sym;
        }
        qi::rule<Iterator, NmeaGLL()> sentence_;
    };
} // namespace nmea::detail