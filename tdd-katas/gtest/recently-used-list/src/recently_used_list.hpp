#ifndef RCL_HPP
#define RCL_HPP

#include <deque>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <limits>

namespace TDD
{
    class MyException : public std::exception {
    };

    class StringList {
    public:

        StringList(uint8_t size = 0) :strList_(), size_(size) {};

        std::deque<std::string> getList() {
            return strList_;
        }

        void addToList(std::string elem)
        {
            if (elem.empty()) {
                throw MyException();
            }

            std::deque<std::string>::iterator it = std::find(strList_.begin(), strList_.end(), elem);
            if (it != strList_.end())
            {
                strList_.erase(it);
            }

            if (strList_.size() == size_)
            {
                strList_.erase(strList_.begin() + size_ - 1);
            }
            strList_.push_front(elem);
        }

        std::string getStrByIndex(uint8_t index)
        {
            return strList_[index];
        }

        uint8_t getMaxSize()
        {
            return size_;
        }
    private:
        std::deque<std::string> strList_;
        uint8_t size_;
    };
}

#endif

