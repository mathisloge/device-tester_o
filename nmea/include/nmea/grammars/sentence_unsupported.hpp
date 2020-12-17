#pragma once
#include "base_grammar.hpp"

BOOST_FUSION_ADAPT_STRUCT(NmeaUnsupported, (Talker, talker)(std::string, message))

namespace nmea::detail
{

    template <typename Iterator>
    struct sentence_unsupported_grammar : public qi::grammar<Iterator, NmeaUnsupported()>
    {
        sentence_unsupported_grammar()
            : sentence_unsupported_grammar::base_type(sentence_)
        {
            sentence_ = kTalkerSymbol >> qi::raw[+(qi::char_ - '*' - qi::space - qi::cntrl)];
        }
        qi::rule<Iterator, NmeaUnsupported()> sentence_;
    };
} // namespace nmea::detail