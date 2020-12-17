#pragma once
#include "base_grammar.hpp"
// clang-format off
BOOST_FUSION_ADAPT_STRUCT(NmeaDTM,
                        (Talker, talker)
                        (DatumRef, datum)
                        (double, offset_latitude)
                        (DirectionIndicator, latitude_indicator)
                        (double, offset_longitude)
                        (DirectionIndicator, longitude_indicator)
                        (double, offset_altitude)
                        (DatumRef, reference_datum))
// clang-format on
namespace nmea::detail
{
    template <typename Iterator>
    struct sentence_dtm_grammar : public qi::grammar<Iterator, NmeaDTM()>
    {
        sentence_dtm_grammar()
            : sentence_dtm_grammar::base_type(sentence_)
        {
            using ascii::string;
            using qi::double_;
            sentence_ = kTalkerSymbol >> "DTM" >> ',' >>
                        kDatumRefSymbol >> ',' >>
                        /*sub_datum*/ ',' >>
                        double_ >> ',' >>
                        kNSSymbol >> ',' >>
                        double_ >> ',' >>
                        kEWSymbol >> ',' >>
                        double_ >> ',' >>
                        kDatumRefSymbol;
        }
        qi::rule<Iterator, NmeaDTM()> sentence_;
    };
} // namespace nmea::detail