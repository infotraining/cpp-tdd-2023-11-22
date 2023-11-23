#include <catch2/catch_test_macros.hpp>
#include "recently_used_list.hpp"
#include <algorithm>
#include <deque>

#include <algorithm>
#include <string>
#include <memory>
#include <vector>

using namespace std;

class CRecentlyUsedList
{
public:
    CRecentlyUsedList()
    {
    }
    const size_t size() const
    {
        return the_best_list_.size();
    }

    void push(const std::string& elem)
    {
        for (auto it = the_best_list_.begin(); it != the_best_list_.end();++it)
        {
            if (*it == elem)
            {
                the_best_list_.erase(it);
                break;
            }
        } 
    
        the_best_list_.emplace_back(elem);
    }

    string pop()
    {
        if(!the_best_list_.empty())
        {
            string value = the_best_list_[the_best_list_.size()-1];
            the_best_list_.pop_back();
            return  value;
        }
        return {};
    }

    string lookup(const size_t index) const
    {
        if(index < the_best_list_.size())
            return the_best_list_[index];
        return {};
    }
    
private:
    std::vector<std::string>the_best_list_;
};


using namespace std;

SCENARIO("Recently used list")
{
    GIVEN("list default creation")
    {
        CRecentlyUsedList list;

        THEN("size is zero")
        {
            REQUIRE(list.size() == 0);
        }

        THEN("lookup returns empty string for invalid index")
        {
            REQUIRE(list.lookup(0) == "");
        }

        WHEN("we add string")
        {
            list.push("red");
            THEN("list size is incremented by one")
            {
                REQUIRE(list.size() == 1);
            }
        }
    }
    GIVEN("list contains 2 strings")
    {
        CRecentlyUsedList list;
        list.push("red");
        list.push("blue");

        THEN("last added string is first taken out")
        {
            REQUIRE(list.pop() == "blue");
            REQUIRE(list.pop() == "red");
        }

        THEN("return empty string if list is empty")
        {
            REQUIRE(list.pop() == "blue");
            REQUIRE(list.pop() == "red");
            REQUIRE(list.pop() == "");
        }

        THEN("each element can be accessed by zero based index")
        {
            list.push("green");
            REQUIRE(list.lookup(1) == "blue");
        }


        THEN("lookup return empty string for invalid index")
        {
            REQUIRE(list.lookup(2) == "");
            REQUIRE(list.lookup(-1) == "");
        }
    }
}


