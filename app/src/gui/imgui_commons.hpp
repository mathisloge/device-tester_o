#pragma once
#include <vector>
namespace gui
{
    bool Button(const char *text, bool disabled = true);

    template <typename T, typename E>
    struct ScrollingBuffer
    {
        int max_size;
        int prev_offset;
        int offset;
        std::vector<T> data;
        ScrollingBuffer(int max_size)
            : max_size{max_size}, prev_offset{-1}, offset{0}
        {
            data.reserve(max_size);
        }

        bool addToHisto(E x, E y)
        {
            if (prev_offset == -1)
            {
                if (data.size() == 0)
                    addPoint(x, y);
                else if (data[data.size() - 1].x == x)
                {
                    data[data.size() - 1].y += y;
                    return false;
                }
                else
                    addPoint(x, y);
                return true;
            }
            if (data[prev_offset].x == x)
            {
                data[prev_offset].y += y;
                return false;
            }
            else
            {
                addPoint(x, y);
                return true;
            }
        }

        void addPoint(E x, E y)
        {
            if (data.size() < max_size)
                data.push_back(T{x, y});
            else
            {
                data[offset] = T{x, y};
                prev_offset = offset;
                offset = (offset + 1) % max_size;
            }
        }

        void erase()
        {
            if (data.size() > 0)
            {
                data.shrink(0);
                prev_offset = -1;
                offset = 0;
            }
        }
    };

} // namespace gui
