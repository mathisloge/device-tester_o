#pragma once
#include "grammars/base_grammar.hpp"
#include "grammars/sentence_dtm.hpp"
#include "grammars/sentence_gll.hpp"

namespace nmea::detail
{
    using AllNmeaMessages = boost::variant<NmeaDTM, NmeaGLL>;

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
    bool parse_nmea(Iterator &first, Iterator last, AllNmeaMessages &msg_instance)
    {
        Iterator mem_first = first;
        Iterator mem_last = last;

        sentence_dtm_grammar<Iterator> sentence_dtm;
        sentence_gll_grammar<Iterator> sentence_gll;
        using qi::parse;
        using qi::uint_parser;
        using qi::skip;

        uint8_t checksum = 0;
        bool r = parse(first, last,
                              //  Begin grammar
                              ('$' >> (sentence_dtm | sentence_gll) >> '*' >> uint_parser<uint8_t, 16, 1, 2>() >> "\r\n"),
                              //  End grammar
                              msg_instance, checksum);

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

} // namespace nmea::detail
