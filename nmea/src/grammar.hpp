#pragma once
#include "grammars/base_grammar.hpp"
#include "grammars/sentence_dtm.hpp"
#include "grammars/sentence_gll.hpp"

BOOST_FUSION_ADAPT_STRUCT(NmeaMessage, (Talker, talker))

using AllNmeaMessages = boost::variant<NmeaDTM, NmeaGLL>;


template <typename Iterator>
struct sentence_gbs_grammar : qi::grammar<Iterator>
{
    sentence_gbs_grammar() : sentence_gbs_grammar::base_type(complete)
    {
        using boost::spirit::ascii::string;
        using boost::spirit::qi::char_;
        using phoenix::ref;
        using qi::_1;
        using qi::double_;

        complete = string("GBS") >> ',' >> time_grammar;
    };
    qi::rule<Iterator> complete;
    time_grammar<Iterator> time_grammar;
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
bool parse_nmea(Iterator first, Iterator last, AllNmeaMessages &msg_instance)
{
    Iterator mem_first = first;
    Iterator mem_last = last;

    sentence_dtm_grammar<Iterator> sentence_dtm;
    sentence_gll_grammar<Iterator> sentence_gll;
    using ascii::space;
    using phoenix::ref;
    using qi::_1;
    using qi::char_;
    using qi::double_;
    using qi::phrase_parse;
    using qi::uint_parser;

    uint8_t checksum = 0;
    bool r = phrase_parse(first, last,
                          //  Begin grammar
                          ('$' >> (sentence_dtm | sentence_gll) >> '*' >> uint_parser<uint8_t, 16, 1, 2>() >> "\r\n"),
                          //  End grammar
                          char_("0"), msg_instance, checksum);

    if (r)
    {
        const auto calc_check = nmea0183_checksum(mem_first, first);
        if (calc_check != checksum)
        {
            std::cout << "checksum mismatch: " << (calc_check == checksum) << " calc: " << static_cast<int>(calc_check) << " got " << static_cast<int>(checksum) << std::endl;
            return false;
        }
    }
    return r;
}
